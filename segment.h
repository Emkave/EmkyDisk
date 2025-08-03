#ifndef SEGMENT_H
#define SEGMENT_H
#include "SFML/Graphics.hpp"
#include "settings.h"
#include <list>

using namespace sf;


namespace EDisk {
    class Segment final : public CircleShape {
    private:
        uintmax_t size;
        const uintmax_t depth = 0;
        const std::wstring filename;

        std::list<Segment> sub_segments;
        std::list<std::wstring> files;

    public:
        explicit Segment(const std::filesystem::path && filename, const uintmax_t && depth);
        void update(void);
        inline std::list<Segment> & get_sub_segment(void) noexcept {return this->sub_segments;}
        inline std::list<std::wstring> & get_files(void) noexcept {return this->files;}
    };
}

#endif //SEGMENT_H
