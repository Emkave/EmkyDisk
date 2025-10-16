#include "disk.h"

namespace EDisk {
    uintmax_t Disk::capacity = 0;
    uintmax_t Disk::available = 0;
    uintmax_t Disk::used = 0;
    float Disk::used_percent = 0;
    std::list<Segment> Disk::segments = {};
}


bool EDisk::Disk::set_path(const std::filesystem::path & path) {
    if (!std::filesystem::is_directory(path)) {
        std::wcerr << "EDisk::Disk::set_path() -> Invalid path given for disk root.\n";
        throw std::runtime_error("EDisk::Disk::set_path() -> Invalid path given for disk root.");
    }

    const std::filesystem::space_info info = std::filesystem::space(path);
    Disk::capacity = info.capacity;
    Disk::available = info.available;
    Disk::used = info.capacity - info.available;
    Disk::used_percent = static_cast<float>(Disk::used) / static_cast<float>(Disk::capacity);

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


size_t EDisk::Disk::set_sizes(Segment * segment, const size_t depth) {
    if (!segment) {
        return 0;
    }

    segment->set_depth(depth);

    size_t size = 0;

    for (Segment *& sub_segment : segment->get_sub_segments()) {
        size += EDisk::Disk::set_sizes(sub_segment, depth+1);
    }

    for (Segment & file : segment->get_files()) {
        size += file.get_size();
    }

    segment->set_size(size);

    return size;
}



bool EDisk::Disk::scan() {
    if (!std::filesystem::exists(registers::scan_path)) {
        return false;
    }


    std::queue<Segment> queue;
    queue.emplace(registers::scan_path, 0, 0);

    while (!queue.empty()) {
        Segment & last_segment = Disk::segments.emplace_back(std::move(queue.front()));
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
                queue.emplace(entry.path().filename(), 0, 0);
                queue.back().set_parent_segment(&last_segment);
            } else {
                last_segment.get_files().emplace_back(entry.path().filename(), EDisk::Disk::get_file_size(entry.path()), 0);
                last_segment.get_files().back().set_parent_segment(&last_segment);
            }
        }
    }

    EDisk::Disk::set_sizes(&(*EDisk::Disk::get_segments().begin()));

    if (!std::ranges::is_sorted(Disk::segments, segment_comparator())) {
        Disk::segments.sort(segment_comparator());
    }

    return true;
}
