#ifndef WINDOW_H
#define WINDOW_H
#include <button.h>
#include <windows.h>

using namespace sf;

class window {
private:
    RectangleButton btn_min;
    RectangleButton btn_max;
    RectangleButton btn_close;
    RectangleShape frame;

    ConvexShape border_shape;

    RenderWindow _win;
    HWND hwnd = nullptr;

    const Color border_color = Color(64, 64, 64);
    const Color bg_color = Color(24, 24, 32);

    static ConvexShape make_round_rect(const Vector2f, float, const int);

public:
    window();
    void assemble();
    void render();
    void handle_events(const Event &);

    inline RenderWindow & win(void) noexcept {return this->_win;}
};


#endif //WINDOW_H
