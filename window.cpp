#include "window.h"
#include <cmath>


ConvexShape window::make_round_rect(const Vector2f size, float r, const int segs) {
    ConvexShape c;

    r = std::max(0.f, std::min(r, std::min(size.x, size.y) * 0.5f));
    const float w = size.x, h = size.y;
    const int n = 4 * segs;
    c.setPointCount(n);

    auto put_arc = [&](const int corner_id, const Vector2f center, const float start_deg) {
        for (int i=0; i<segs; i++) {
            const float t = (segs == 1) ? 0.f : static_cast<float>(i) / static_cast<float>(segs - 1);
            const float deg = start_deg + t * 90.f;
            const float rad = deg * 3.1415926535f / 180.f;
            const Vector2f p = center + Vector2f(std::cos(rad), std::sin(rad)) * r;
            c.setPoint(corner_id * segs + i, p);
        }
    };

    const Vector2f tl(r, r);
    const Vector2f tr(w-r, r);
    const Vector2f br(w-r, h-r);
    const Vector2f bl(r, h-r);

    put_arc(0, tl, 180.f); // TL: 180->270
    put_arc(1, tr, 270.f); // TR: 270->360
    put_arc(2, br,   0.f); // BR:   0-> 90
    put_arc(3, bl,  90.f); // BL:  90->180
    
    return c;
}


void window::init() {
    ContextSettings settings;
    settings.antiAliasingLevel = 10;

    this->_win.create(VideoMode({900, 600}), "EmkyDisk", Style::None, State::Windowed, settings);
    this->_win.setFramerateLimit(120);
    this->hwnd = reinterpret_cast<HWND>(this->_win.getNativeHandle());

    RECT rect;
    GetClientRect(this->hwnd, &rect);
    const int w = rect.right - rect.left;
    const int h = rect.bottom - rect.top;
    const HRGN region = CreateRoundRectRgn(0, 0, w+1, h+1, this->corner_radius * 1.f-3, this->corner_radius * 1.f-3);
    SetWindowRgn(this->hwnd, region, TRUE);
    DeleteObject(region);

    constexpr float btn_w = 46.f;
    constexpr float btn_h = 26.f;

    this->border_shape = window::make_round_rect(Vector2f(w, h), this->corner_radius*1.f-11, 60);
    this->border_shape.setPosition({0.f, 0.f});
    this->border_shape.setOutlineColor(this->border_color);
    this->border_shape.setOutlineThickness(-1);
    this->border_shape.setFillColor(Color::Transparent);

    this->btn_min.setSize({btn_w, btn_h});
    this->btn_min.setFillColor(Color(50*0.7, 50*0.7, 66*0.7));
    this->btn_min.setPosition({w - 3*btn_w, 0});

    this->btn_max.setSize({btn_w, btn_h});
    this->btn_max.setFillColor(Color(50*0.7, 50*0.7, 66*0.7));
    this->btn_max.setPosition({w - 2*btn_w, 0});

    this->btn_close.setSize({btn_w, btn_h});
    this->btn_close.setFillColor(Color(50*0.7, 50*0.7, 66*0.7));
    this->btn_close.setPosition({w - btn_w, 0});
}


void window::render() {
    this->_win.draw(this->btn_close);
    this->_win.draw(this->btn_max);
    this->_win.draw(this->btn_min);
    _win.draw(border_shape);
}
