#include "disk.h"


bool EDisk::Disk::set_path(const std::filesystem::path & path) {
    if (!std::filesystem::is_directory(path)) {
        std::wcerr << "EDisk::Disk::set_path() -> Invalid path given for disk root.\n";
        throw std::runtime_error("EDisk::Disk::set_path() -> Invalid path given for disk root.");
    }
    this->root = path;

    const std::filesystem::space_info info = std::filesystem::space(path);
    this->capacity = info.capacity;
    this->available = info.available;
    this->used = info.capacity - info.available;
    this->used_percent = static_cast<float>(this->used) / static_cast<float>(this->capacity);

    return true;
}


uintmax_t EDisk::Disk::get_depth(const std::filesystem::path & root, const std::filesystem::path & target) {
    const std::filesystem::path relative = std::filesystem::relative(target, root);
    return std::distance(relative.begin(), relative.end());
}


size_t EDisk::Disk::get_file_size(const std::filesystem::path &path) {
    LARGE_INTEGER size;
    DWORD high;
    const DWORD low = GetCompressedFileSizeW(path.c_str(), &high);

    if (low == INVALID_FILE_SIZE && GetLastError() != NO_ERROR) {
        return 0;
    }

    size.LowPart = low;
    size.HighPart = static_cast<long>(high);

    return static_cast<size_t>(size.QuadPart);
}


size_t EDisk::Disk::get_size(const std::filesystem::path & path) {
    if (std::filesystem::is_directory(path)) {
        size_t total = 0;
        for (const std::filesystem::directory_entry & entry : std::filesystem::recursive_directory_iterator(path, std::filesystem::directory_options::skip_permission_denied)) {
            total += EDisk::Disk::get_file_size(entry.path());
        }
        return total;
    } else {
        return EDisk::Disk::get_file_size(path);
    }
}


bool EDisk::Disk::scan() {
    if (!std::filesystem::exists(this->root)) {
        return false;
    }

    std::queue<Segment> queue;
    queue.emplace(this->root, EDisk::Disk::get_size(this->root), EDisk::Disk::get_depth(this->root, this->root));

    while (!queue.empty()) {
        Segment & last_segment = this->segments.emplace_back(std::move(queue.front()));
        queue.pop();

        std::filesystem::path iterator_dest_path = last_segment.get_path();

        if (last_segment.get_parent()) {
            last_segment.get_parent()->get_sub_segments().emplace_back(&last_segment);
            iterator_dest_path = last_segment.construct_path();

            if (iterator_dest_path == "") {
                continue;
            }
        }

        for (const std::filesystem::directory_entry & entry : std::filesystem::directory_iterator(iterator_dest_path, std::filesystem::directory_options::skip_permission_denied)) {
            if (entry.is_directory()) {
                queue.emplace(entry.path().filename(), EDisk::Disk::get_size(entry.path()), EDisk::Disk::get_depth(this->root, entry.path()));
                queue.back().set_parent_segment(&last_segment);
            } else {
                last_segment.get_files().emplace_back(entry.path().filename(), EDisk::Disk::get_file_size(entry.path()), EDisk::Disk::get_depth(this->root, entry.path()));
                last_segment.get_files().back().set_parent_segment(&last_segment);
            }
        }
    }

    for (Segment & segment : this->segments) {
        std::filesystem::path path_check = segment.construct_path();

        if (!std::filesystem::exists(path_check)) {
            std::wcerr << "EDisk::Disk::scan() -> the path to " << path_check.wstring() << " is corrupted.\n";
            //throw std::runtime_error("EDisk::Disk::scan() -> the path to " + path_check.wstring() + " is corrupted.");
        }

        for (Segment & file : segment.get_files()) {
            path_check = file.construct_path();

            if (!std::filesystem::exists(path_check)) {
                std::wcerr << "EDisk::Disk::scan() -> the path to " << path_check.wstring() << " is corrupted.\n";
                //throw std::runtime_error("EDisk::Disk::scan() -> the path to " + path_check.wstring() + " is corrupted.");
            }
        }
    }


    if (!std::ranges::is_sorted(this->segments, segment_comparator())) {
        this->segments.sort(segment_comparator());
    }

    return true;
}
