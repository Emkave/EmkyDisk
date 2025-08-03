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
    if (!target.has_root_path() || !root.has_root_path()) {
        throw std::runtime_error("One of the paths is not absolute.");
    }

    if (root.root_name() != target.root_name()) {
        throw std::runtime_error("Paths are on different drives: " + root.string() + " vs " + target.string());
    }

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

        Segment segment (current.filename(), EDisk::Disk::get_depth(current.relative_path(), this->root));

        for (const std::filesystem::directory_entry & entry : std::filesystem::directory_iterator(current)) {
            if (entry.is_directory()) {
                queue.emplace(entry.path());
                segment.get_sub_segment().emplace_back(entry.path().filename(), EDisk::Disk::get_depth(entry.path(), this->root));
            } else {
                segment.get_files().emplace_back(entry.path().wstring());
            }
        }

        this->disk.try_emplace(current.wstring(), segment);
    }

    return true;
}
