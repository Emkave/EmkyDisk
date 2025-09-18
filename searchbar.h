#ifndef SEARCHBAR_H
#define SEARCHBAR_H
#include <SFML/Graphics.hpp>
#include "settings.h"

using namespace sf;

class searchbar {
private:
    ConvexShape shape;

    Text text = Text(registers::font, registers::init_scan_path);

public:
    searchbar();

    inline ConvexShape & get_shape(void) noexcept {return this->shape;}
    inline Text & get_text(void) noexcept {return this->text;}



    void draw() const;
    void update();
    void handle_event(const Event &);

    void set_position(const Vector2f pos);

};

#endif //SEARCHBAR_H
