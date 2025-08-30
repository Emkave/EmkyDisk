#ifndef WINDOW_H
#define WINDOW_H
#include <SFML/Graphics.hpp>
#include <windows.h>

using namespace sf;

class window {
private:
    RectangleShape btn_min;
    RectangleShape btn_max;
    RectangleShape btn_close;

    RenderWindow _win;
    HWND hwnd = nullptr;

    ConvexShape border_shape;
    ConvexShape background_shape;

    float caption_h = 36.f;
    float btn_w = 46.f;
    float corner_radius = 20.f;
    float border_thickness = 2.f;

    Color border_color = Color(64, 64, 64);
    Color bg_color = Color(24, 24, 32);

    static ConvexShape make_round_rect(const Vector2f, float, const int);

public:
    void init();
    void render();

    inline RenderWindow & win(void) noexcept {return this->_win;}
};


#endif //WINDOW_H
