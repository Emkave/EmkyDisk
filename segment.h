#ifndef SEGMENT_H
#define SEGMENT_H
#include "SFML/Graphics.hpp"
#include "settings.h"
#include <list>

using namespace sf;


namespace EDisk {
    class Segment {
    private:
        CircleShape * shape = nullptr;

        uintmax_t depth = 0;
        size_t size = 0;
        std::filesystem::path path;
        Segment * parent = nullptr;

        std::list<Segment *> sub_segments;
        std::list<Segment> files;

    public:
        explicit Segment(std::filesystem::path path, size_t size, uintmax_t depth);
        void update(void);
        std::filesystem::path construct_path(void) const;

        inline std::list<Segment *> & get_sub_segments(void) noexcept {return this->sub_segments;}
        inline std::list<Segment> & get_files(void) noexcept {return this->files;}
        inline uintmax_t get_depth(void) const noexcept {return this->depth;}
        inline const std::filesystem::path & get_path(void) noexcept {return this->path;}
        inline Segment * get_parent(void) const noexcept {return this->parent;}

        inline void set_parent_segment(Segment * __parent) noexcept {this->parent = __parent;}
    };
}

#endif //SEGMENT_H
