#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>
#include "settings.h"
#include <functional>
#include <type_traits>

#include "disk.h"

using namespace sf;

template <typename ShapeT> class button {
    static_assert(std::is_base_of_v<Shape, ShapeT>, "ShapeT must derive from Shape.");
    ShapeT shape;
    Color shape_base_color = Color(50*0.7f, 50*0.7f, 66.f*0.7f);
    Color shape_hover_color = Color(50*0.9f, 50*0.9f, 66.f*0.9f);
    Color shape_press_color = Color(50*0.9f, 50*0.9f, 66.f*0.9f);

    Color text_base_color = Color(180, 180, 180);
    Color text_hover_color = Color(180, 180, 180);
    Color text_press_color = Color(180, 180, 180);

    Text text = Text(registers::font);

    bool hovered = false;
    bool pressed = false;
    bool pressed_inside = false;

    std::function<void(void)> action = []{};
    static inline Vector2f mouse_world(void);
    inline bool hit(const Vector2f &) const;
    void apply_colors();

public:
    template <typename ... Args> explicit button(Args && ... args) : shape(std::forward<Args>(args)...) {}

    inline ShapeT & get_shape(void) noexcept {return this->shape;}
    inline Text & get_text(void) noexcept {return this->text;}
    inline void set_shape_base_color(const Color && color) noexcept {this->shape_base_color = color;}
    inline void set_shape_hover_color(const Color && color) noexcept {this->shape_hover_color = color;}
    inline void set_shape_press_color(const Color && color) noexcept {this->shape_press_color = color;}
    inline void set_text_base_color(const Color && color) noexcept {this->text_base_color = color;}
    inline void set_text_hover_color(const Color && color) noexcept {this->text_hover_color = color;}
    inline void set_text_press_color(const Color && color) noexcept {this->text_press_color = color;}

    void set_action(std::function<void(void)> a) {this->action = a;}
    void handle_event(const Event &);
    void execute() const {this->action();}
    void render();
};


template<typename ShapeT> Vector2f button<ShapeT>::mouse_world() {
    return registers::window->mapPixelToCoords(Mouse::getPosition(*registers::window));
}


template<typename ShapeT> bool button<ShapeT>::hit(const Vector2f & p) const {
    return this->shape.getGlobalBounds().contains(p);
}


template<typename ShapeT> void button<ShapeT>::apply_colors() {
    if (this->pressed && this->pressed_inside) {
        this->shape.setFillColor(this->hovered ? this->shape_press_color : this->shape_base_color);
        this->text.setFillColor(this->text_press_color);
    } else if (this->hovered) {
        this->shape.setFillColor(this->shape_hover_color);
        this->text.setFillColor(this->text_hover_color);
    } else {
        this->shape.setFillColor(this->shape_base_color);
        this->text.setFillColor(this->text_base_color);
    }
}


template<typename ShapeT> void button<ShapeT>::handle_event(const Event & event) {
    const Vector2f mp = this->mouse_world();

    if (this->hit(mp)) {
        this->hovered = true;
        this->apply_colors();

        if (const auto * mbe = event.getIf<Event::MouseButtonPressed>()) {
            if (mbe->button == Mouse::Button::Left) {
                this->pressed = true;
                this->pressed_inside = true;
                SetCapture(reinterpret_cast<HWND>(registers::window->getNativeHandle()));
                this->apply_colors();
            }
        }

        if (const auto * mre = event.getIf<Event::MouseButtonReleased>()) {
            if (mre->button == Mouse::Button::Left) {
                const bool started_inside = this->pressed_inside;
                this->pressed = false;
                this->pressed_inside = false;
                ReleaseCapture();
                this->apply_colors();

                if (started_inside) {
                    this->execute();
                }
            }
        }
    } else {
        this->hovered = false;
        this->pressed = false;
        this->pressed_inside = false;
        this->apply_colors();
    }
}


template<typename ShapeT> void button<ShapeT>::render() {
    this->apply_colors();
}


using RectangleButton = button<RectangleShape>;
using CircleButton = button<CircleShape>;
using ConvexButton = button<ConvexShape>;

#endif //BUTTON_H
