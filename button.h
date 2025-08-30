#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>
#include <settings.h>
#include <functional>
#include <type_traits>

using namespace sf;

template <typename ShapeT> class button {
    static_assert(std::is_base_of_v<Shape, ShapeT>, "ShapeT must derive from Shape.");
    ShapeT shape;
    Color base_color = Color(50*0.7, 50*0.7, 66*0.7);
    Color hover_color = Color(50*0.9, 50*0.9, 66*0.9);
    Color press_color = Color(50*0.4, 50*0.4, 66*0.4);

    std::function<void(void)> action = []{};
    bool is_hovered();
public:
    template <typename ... Args> explicit button(Args && ... args) : shape(std::forward<Args>(args)...) {}
    inline ShapeT & get(void) noexcept {return this->shape;}

    void render();
    void set_base_color(const Color & color) noexcept {this->base_color = color;}
    void set_hover_color(const Color & color) noexcept {this->hover_color = color;}
    void set_press_color(const Color & color) noexcept {this->press_color = color;}
    void set_action(std::function<void(void)> a) {this->action = a;}
    void execute() const {this->action();}
};


template<typename ShapeT> bool button<ShapeT>::is_hovered() {
    return this->shape.getGlobalBounds().contains(static_cast<Vector2f>(Mouse::getPosition(*registers::window)));
}

template<typename ShapeT> void button<ShapeT>::render() {
    if (this->is_hovered()) {
        this->shape.setFillColor(this->hover_color);
    } else if (this->shape.getFillColor() != this->base_color) {
        this->shape.setFillColor(this->base_color);
    }
}


using RectangleButton = button<RectangleShape>;
using CircleButton = button<CircleShape>;
using ConvexButton = button<ConvexShape>;

#endif //BUTTON_H
