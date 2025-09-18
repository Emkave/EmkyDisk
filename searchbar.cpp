#include "searchbar.h"
#include "other.h"


searchbar::searchbar() {
    this->shape = functions::make_round_rect({700, 23}, 6.f, 24);
    this->text.setFillColor(Color(136, 136, 136));
    this->text.setCharacterSize(17);
}


void searchbar::draw() const {
    registers::window->draw(this->shape);
    registers::window->draw(this->text);
}


void searchbar::update() {

}


void searchbar::handle_event(const Event &) {

}


void searchbar::set_position(const Vector2f pos) {
    this->shape.setPosition(pos);

    this->text.setOrigin(this->text.getGlobalBounds().getCenter());
    const Vector2f c = this->shape.getTransform().transformPoint(this->shape.getLocalBounds().getCenter());
    this->text.setPosition(c + Vector2f(-330.f, 0.f));
}
