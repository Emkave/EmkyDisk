#ifndef SEARCHBAR_H
#define SEARCHBAR_H
#include <SFML/Graphics.hpp>
#include "settings.h"

using namespace sf;

class searchbar {
private:
    ConvexShape shape;
    Text inline_text = Text(registers::font, registers::init_scan_path);

public:
    inline ConvexShape & get_shape(void) noexcept {return this->shape;}
    inline Text & get_text(void) noexcept {return this->inline_text;}
};

#endif //SEARCHBAR_H
