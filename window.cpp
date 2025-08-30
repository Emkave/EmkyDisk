#include "window.h"
#include <settings.h>
#include <cmath>


window::window() {
    ContextSettings settings;
    settings.antiAliasingLevel = 10;
    this->_win.create(VideoMode({WIN_WIDTH, WIN_HEIGHT}), "EmkyDisk", Style::None, State::Windowed, settings);
    this->_win.setFramerateLimit(120);
    registers::window = &this->_win;
}


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


void window::handle_events(const Event & event) {
    if (!event.is<sf::Event::MouseButtonPressed>()) {
        return;
    }
    const Event::MouseButtonPressed * mbe = event.getIf<Event::MouseButtonPressed>();

    if (!mbe || mbe->button != Mouse::Button::Left) {
        return;
    }

    const Vector2f mpf = static_cast<Vector2f>(Mouse::getPosition(this->_win));

    if (this->btn_close.get().getGlobalBounds().contains(mpf)) {
        this->btn_close.execute();
    }
    if (this->btn_max.get().getGlobalBounds().contains(mpf)) {
        this->btn_max.execute();
    }
    if (this->btn_min.get().getGlobalBounds().contains(mpf)) {
        this->btn_min.execute();
    }

    if (mpf.y >= 0 && mpf.y < 26.f) {
        ReleaseCapture();
        SendMessage(this->hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
    }
}


void window::assemble() {
    this->hwnd = reinterpret_cast<HWND>(this->_win.getNativeHandle());

    RECT rect;
    GetClientRect(this->hwnd, &rect);
    const int w = rect.right - rect.left;
    const int h = rect.bottom - rect.top;
    const HRGN region = CreateRoundRectRgn(0, 0, w+1, h+1, 20.f * 1.f-3, 20.f*1.f-3);
    SetWindowRgn(this->hwnd, region, TRUE);
    DeleteObject(region);

    constexpr float btn_w = 46.f;
    constexpr float btn_h = 26.f;

    this->border_shape = window::make_round_rect(Vector2f(w, h), 20.f*1.f-11, 60);
    this->border_shape.setPosition({0.f, 0.f});
    this->border_shape.setOutlineColor(this->border_color);
    this->border_shape.setOutlineThickness(-1);
    this->border_shape.setFillColor(Color::Transparent);

    this->btn_min.get().setSize({btn_w, btn_h});
    this->btn_min.get().setFillColor(Color(50*0.7, 50*0.7, 66*0.7));
    this->btn_min.get().setPosition({w - 3*btn_w, 0});

    this->btn_max.get().setSize({btn_w, btn_h});
    this->btn_max.get().setFillColor(Color(50*0.7, 50*0.7, 66*0.7));
    this->btn_max.get().setPosition({w - 2*btn_w, 0});

    this->btn_close.get().setSize({btn_w, btn_h});
    this->btn_close.get().setFillColor(Color(50*0.7, 50*0.7, 66*0.7));
    this->btn_close.get().setPosition({w - btn_w, 0});
    this->btn_close.set_hover_color(Color(200, 46, 46));
    this->btn_close.set_action([&]{this->_win.close();});

    this->frame.setPosition({0, 0});
    this->frame.setFillColor(Color(50*0.7, 50*0.7, 66*0.7));
    this->frame.setSize({WIN_WIDTH-btn_w*3, btn_h});
}


void window::render() {
    this->btn_close.render();
    this->btn_max.render();
    this->btn_min.render();

    this->_win.draw(this->btn_close.get());
    this->_win.draw(this->btn_max.get());
    this->_win.draw(this->btn_min.get());
    this->_win.draw(this->frame);
    this->_win.draw(border_shape);
}
