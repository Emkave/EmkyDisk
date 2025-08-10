#include "disk.h"


bool EDisk::Disk::set_path(const std::filesystem::path & path) {
    if (!std::filesystem::is_directory(path)) {
        return false;
    }
    this->root = path;

    const std::filesystem::space_info info = std::filesystem::space(path);
    this->capacity = info.capacity;
    this->available = info.available;
    this->used = info.capacity - info.available;
    this->used_percent = static_cast<float>(this->used) / static_cast<float>(this->capacity);

    return true;
}


uintmax_t EDisk::Disk::get_depth(const std::filesystem::path & target, const std::filesystem::path & root) {
    const std::filesystem::path relative = std::filesystem::relative(target, root);
    return std::distance(relative.begin(), relative.end());
}


bool EDisk::Disk::scan() {
    if (!std::filesystem::exists(this->root)) {
        return false;
    }

    std::queue<std::filesystem::path> queue;
    queue.emplace(this->root);

    while (!queue.empty()) {
        std::filesystem::path current = queue.front();
        queue.pop();

        Segment segment (current, EDisk::Disk::get_depth(current.relative_path(), this->root.relative_path()));

        for (const std::filesystem::directory_entry & entry : std::filesystem::directory_iterator(current)) {
            if (entry.is_directory()) {
                queue.emplace(entry.path());
                segment.get_sub_segment().emplace_back(entry.path().filename().wstring());
            } else {
                segment.get_files().emplace_back(entry.path().filename().wstring());
            }
        }

        this->disk.try_emplace(current, segment);
        this->segments.emplace_back(&this->disk.at(current.wstring()));
    }

    if (!std::ranges::is_sorted(this->segments, segment_comparator())) {
        this->segments.sort(segment_comparator());
    }

    for (std::pair<const std::wstring, Segment> & iter : this->disk) {
        if (!std::filesystem::exists(iter.first)) {
            std::wcerr << "disk::scan() -> corrupted directory path. Something went wrong with " << iter.first << std::endl;
        }

        for (std::wstring & str : iter.second.get_files()) {
            if (!std::filesystem::exists(std::wstring(iter.first + "/" + str))) {
                std::wcerr << "disk::scan() -> corrupted file path. Something went wrong with " << iter.first + str << std::endl;
            }
        }
    }

    return true;
}
