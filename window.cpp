#include "window.h"
#include "settings.h"
#include "other.h"


window::window() {
    ContextSettings settings;
    settings.antiAliasingLevel = 5;
    this->_win.create(VideoMode({WIN_WIDTH, WIN_HEIGHT}), "EmkyDisk", Style::None, State::Windowed, settings);
    this->_win.setFramerateLimit(120);
    registers::window = &this->_win;
    this->hwnd = reinterpret_cast<HWND>(this->_win.getNativeHandle());
    LONG_PTR style = GetWindowLongPtr(this->hwnd, GWL_STYLE);
    style |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
    SetWindowLongPtr(this->hwnd, GWL_STYLE, style);
    LONG_PTR ex = GetWindowLongPtr(this->hwnd, GWL_EXSTYLE);
    ex |= WS_EX_APPWINDOW;
    SetWindowLongPtr(this->hwnd, GWL_EXSTYLE, ex);
    SetWindowPos(this->hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}


void window::resize_window_after_scan() const {
    RECT rect;
    GetWindowRect(this->hwnd, &rect);

    const int width = rect.right - rect.left;
    constexpr int height = 600;

    SetWindowPos(this->hwnd, nullptr, rect.left, rect.top, width, height, SWP_NOZORDER | SWP_NOACTIVATE);

    const HRGN region = CreateRoundRectRgn(0, 0, width+1, height+1, 20, 20);
    SetWindowRgn(this->hwnd, region, TRUE);
    DeleteObject(region);
}


void window::handle_events(const Event & event) {
    this->btn_close.handle_event(event);
    this->btn_max.handle_event(event);
    this->btn_min.handle_event(event);
    this->btn_browse.handle_event(event);
    this->btn_scan.handle_event(event);
    //this->searchbar.handle_event(event);

    if (const auto * mbe = event.getIf<Event::MouseButtonPressed>()) {
        if (mbe->button == Mouse::Button::Left) {
            const Vector2f mp = this->_win.mapPixelToCoords(Mouse::getPosition(this->_win));

            auto over_any = [&](const Vector2f p) -> bool {
                return this->btn_close.get_shape().getGlobalBounds().contains(p) ||
                       this->btn_max.get_shape().getGlobalBounds().contains(p) ||
                       this->btn_min.get_shape().getGlobalBounds().contains(p) ||
                       this->btn_browse.get_shape().getGlobalBounds().contains(p) ||
                       this->btn_scan.get_shape().getGlobalBounds().contains(p);
            };

            if (!over_any(mp) && mp.y >= 0.f && mp.y < 26.f) {
                ReleaseCapture();
                SendMessage(this->hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            }
        }
    }
}


void window::reassemble() {
    RECT rect;
    GetClientRect(this->hwnd, &rect);
    const int w = rect.right - rect.left;
    const int h = rect.bottom - rect.top;
    const HRGN region = CreateRoundRectRgn(0, 0, w+1, h+1, 20.f * 1.f-3, 20.f*1.f-3);
    SetWindowRgn(this->hwnd, region, TRUE);
    DeleteObject(region);

    this->border_shape = functions::make_round_rect(Vector2f(static_cast<float>(w), static_cast<float>(h)), 9, 60);
    this->border_shape.setPosition({0.f, 0.f});
    this->border_shape.setOutlineColor(this->border_color);
    this->border_shape.setOutlineThickness(-1);
    this->border_shape.setFillColor(Color::Transparent);
}


void window::assemble() {
    RECT rect;
    GetClientRect(this->hwnd, &rect);
    const int w = rect.right - rect.left;
    const int h = rect.bottom - rect.top;
    const HRGN region = CreateRoundRectRgn(0, 0, w+1, h+1, 20.f * 1.f-3, 20.f*1.f-3);
    SetWindowRgn(this->hwnd, region, TRUE);
    DeleteObject(region);

    constexpr float btn_w = 46.f;
    constexpr float btn_h = 26.f;

    this->border_shape = functions::make_round_rect(Vector2f(static_cast<float>(w), static_cast<float>(h)), 9, 60);
    this->border_shape.setPosition({0.f, 0.f});
    this->border_shape.setOutlineColor(this->border_color);
    this->border_shape.setOutlineThickness(-1);
    this->border_shape.setFillColor(Color::Transparent);

    this->btn_min.get_shape().setSize({btn_w, btn_h});
    this->btn_min.get_shape().setFillColor(Color(35, 35, 46));
    this->btn_min.get_shape().setPosition({static_cast<float>(w) - 2*btn_w, 0});
    this->btn_min.set_action([&]{ShowWindow(this->hwnd, SW_MINIMIZE);});
    this->btn_min.set_label("_");
    this->btn_min.set_text_size(20);
    this->btn_min.set_text_base_color(Color(70, 70, 86));
    this->btn_min.set_text_hover_color(Color(255, 255, 255));

    this->btn_close.get_shape().setSize({btn_w, btn_h});
    this->btn_close.get_shape().setFillColor(Color(35, 35, 46));
    this->btn_close.get_shape().setPosition({static_cast<float>(w) - btn_w, 0});
    this->btn_close.set_shape_hover_color(Color(200, 46, 46));
    this->btn_close.set_action([&]{this->_win.close();});
    this->btn_close.set_label(L"˟");
    this->btn_close.set_text_size(30);
    this->btn_close.set_text_base_color(Color(70, 70, 86));
    this->btn_close.set_text_hover_color(Color(255, 255, 255));

    this->btn_browse.get_shape() = functions::make_round_rect({70, 23}, 6, 30);
    this->btn_browse.get_shape().setPosition({730, 40});
    this->btn_browse.get_shape().setFillColor(Color(45, 45, 56));
    this->btn_browse.set_shape_press_color(Color(30, 30, 40));
    this->btn_browse.set_label("...");
    this->btn_browse.set_text_size(20);
    this->btn_browse.set_text_base_color(Color(100, 100, 100));
    this->btn_browse.set_text_press_color(Color(250, 250, 250));
    this->btn_browse.set_action([this] {
        if (const std::optional<std::wstring> path = functions::browse_folder(this->hwnd)) {
            registers::init_scan_path = *path;
            this->searchbar.get_text().setString(registers::init_scan_path);
            this->searchbar.reindex();
        }
    });

    this->btn_scan.get_shape() = functions::make_round_rect({70, 23}, 6, 30);
    this->btn_scan.get_shape().setPosition({820, 40});
    this->btn_scan.get_shape().setFillColor(Color(45, 45, 56));
    this->btn_scan.set_shape_press_color(Color(30, 30, 40));
    this->btn_scan.set_label("Scan");
    this->btn_scan.set_text_size(13);
    this->btn_scan.set_text_base_color(Color(100, 100, 100));
    this->btn_scan.set_text_press_color(Color(250, 250, 250));
    this->btn_scan.set_action(std::bind_front(&window::resize_window_after_scan, this));

    this->frame.setPosition({0, 0});
    this->frame.setFillColor(Color(35, 35, 46));
    this->frame.setSize({WIN_WIDTH-btn_w*2, btn_h});

    this->title.setCharacterSize(12);
    this->title.setFillColor(Color(180, 180, 180));
    this->title.setPosition({static_cast<float>(WIN_WIDTH)/2-6, 6});

    this->searchbar.get_shape() = functions::make_round_rect({700, 23}, 6.f, 24);
    this->searchbar.get_shape().setFillColor(Color(45, 45, 56));
    this->searchbar.get_text().setFillColor(Color(190, 190, 190));
    this->searchbar.get_text().setCharacterSize(15);
    this->searchbar.get_text().setLetterSpacing(1.4992826398852224);
    this->searchbar.set_position({10, 40});
}


void window::render() {
    this->btn_close.render();
    this->btn_max.render();
    this->btn_min.render();
    this->btn_browse.render();
    this->btn_scan.render();

    this->btn_close.draw();
    this->btn_min.draw();
    this->btn_browse.draw();
    this->btn_scan.draw();
    this->searchbar.draw();

    this->_win.draw(this->frame);
    this->_win.draw(this->title);
    this->_win.draw(this->border_shape);
}
