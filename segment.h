#ifndef SEGMENT_H
#define SEGMENT_H
#include "SFML/Graphics.hpp"
#include "settings.h"
#include <list>

using namespace sf;


namespace EDisk {
    class Segment;



    class Segment final : public CircleShape {
    private:
        const uintmax_t depth = 0;
        const std::filesystem::path path;

        std::list<std::wstring> sub_segments;
        std::list<std::wstring> files;

    public:
        explicit Segment(std::filesystem::path path, const uintmax_t && depth);
        void update(void);
        inline std::list<std::wstring> & get_sub_segment(void) noexcept {return this->sub_segments;}
        inline std::list<std::wstring> & get_files(void) noexcept {return this->files;}
        inline uintmax_t get_depth(void) const noexcept {return this->depth;}
    };
}

#endif //SEGMENT_H
