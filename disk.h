#ifndef DISK_H
#define DISK_H
#if WIN32
#include <windows.h>
#endif
#include <SFML/Graphics.hpp>
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
        static uintmax_t capacity;
        static uintmax_t available;
        static uintmax_t used;
        static float used_percent;

        //std::unordered_map<std::wstring, Segment> disk;
        static std::list<Segment> segments;

        static size_t get_file_size(const std::filesystem::path & path);
        static size_t set_sizes(Segment * segment, size_t depth = 1);

    public:
        static bool set_path(const std::filesystem::path & path);
        static bool scan();

        static uintmax_t get_depth(const std::filesystem::path & root, const std::filesystem::path & target);
        static size_t get_size(const std::filesystem::path & path);

        static inline uintmax_t get_capacity(void) noexcept {return Disk::capacity;}
        static inline uintmax_t get_available(void) noexcept {return Disk::available;}
        static inline uintmax_t get_used(void) noexcept {return Disk::used;}
        static inline float get_usage_percent(void) noexcept {return Disk::used_percent;}
        //inline std::unordered_map<std::wstring, Segment> & get_disk(void) noexcept {return this->disk;}
        static inline std::list<Segment> & get_segments(void) noexcept {return Disk::segments;}
    };
}



#endif //DISK_H
