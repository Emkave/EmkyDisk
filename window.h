#ifndef WINDOW_H
#define WINDOW_H
#include "button.h"
#include "other.h"
#include "searchbar.h"
#include <windows.h>


using namespace sf;

class window {
private:
    RectangleButton btn_min;
    RectangleButton btn_max;
    RectangleButton btn_close;
    ConvexButton btn_browse;
    ConvexButton btn_scan;
    RectangleShape frame;
    ConvexShape border_shape;

    searchbar searchbar;

    Font font = Font("../arial.ttf");
    Text title = Text(font, "EmkyDisk");

    RenderWindow _win;
    HWND hwnd = nullptr;

    const Color border_color = Color(64, 64, 64);
    const Color bg_color = Color(24, 24, 32);

    template <typename Func> static auto make_function(Func ptr) {
        return std::function<std::remove_pointer_t<Func>>(ptr);
    }

public:
    window();
    void assemble();
    void render();
    void handle_events(const Event &);
    void resize_window_after_scan(void) const;

    inline RenderWindow & win(void) noexcept {return this->_win;}
};


#endif //WINDOW_H
