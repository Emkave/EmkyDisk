#ifndef SEARCHBAR_H
#define SEARCHBAR_H
#include <SFML/Graphics.hpp>
#include "settings.h"

using namespace sf;

class searchbar {
private:
    ConvexShape shape;

    Text text = Text(registers::font, registers::init_scan_path);

    short begin_index = 0;
    short end_index = 0;
    short caret_index = 0;
    bool activated = false;

    char caret_symbol = '|';
    Text caret_text = Text(registers::font);

    const short field_size = 91;

    static Vector2f mouse_world();
    bool hit(const Vector2f &) const;
public:
    searchbar();

    inline ConvexShape & get_shape(void) noexcept {return this->shape;}
    inline Text & get_text(void) noexcept {return this->text;}


    void draw() const;
    void update();
    void reindex();
    void handle_event(const Event &);
    void set_position(Vector2f);
};

#endif //SEARCHBAR_H
