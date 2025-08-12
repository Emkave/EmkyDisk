#ifndef DISK_H
#define DISK_H
#if WIN32
#include <windows.h>
#endif
#include "SFML/Graphics.hpp"
#include "settings.h"
#include "segment.h"
#include <filesystem>
#include <queue>
#include <iostream>


using namespace sf;

namespace EDisk {
    struct segment_comparator {
        bool operator ()(const Segment & a, const Segment & b) const {
            return a.get_depth() < b.get_depth();
        }
    };

    class Disk {
    private:
        uintmax_t capacity = 0;
        uintmax_t available = 0;
        uintmax_t used = 0;
        float used_percent = 0;

        std::filesystem::path root;

        //std::unordered_map<std::wstring, Segment> disk;
        std::list<Segment> segments;

        static size_t get_file_size(const std::filesystem::path & path);

    public:
        bool set_path(const std::filesystem::path & path);
        bool scan();

        static uintmax_t get_depth(const std::filesystem::path & root, const std::filesystem::path & target);
        static size_t get_size(const std::filesystem::path & path);

        inline uintmax_t get_capacity(void) const noexcept {return this->capacity;}
        inline uintmax_t get_available(void) const noexcept {return this->available;}
        inline uintmax_t get_used(void) const noexcept {return this->used;}
        inline float get_usage_percent(void) const noexcept {return this->used_percent;}
        //inline std::unordered_map<std::wstring, Segment> & get_disk(void) noexcept {return this->disk;}
        inline std::list<Segment> & get_segments(void) noexcept {return this->segments;}
    };
}

#endif //DISK_H
