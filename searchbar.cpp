#include "searchbar.h"
#include "other.h"


searchbar::searchbar() {
    this->shape = functions::make_round_rect({700, 23}, 6.f, 24);
    this->text.setFillColor(Color(136, 136, 136));
    this->text.setCharacterSize(17);
    this->caret_text = Text(registers::font, this->caret_symbol);
    this->caret_text.setPosition({this->text.getPosition().x, this->text.getPosition().y});
}


void searchbar::draw() const {
    registers::window->draw(this->shape);
    registers::window->draw(this->text);

    if (this->activated) {
        registers::window->draw(this->caret_text);
    }
}


void searchbar::update() {
    this->text.setString(std::move(registers::init_scan_path.substr(this->begin_index, this->end_index+1)));
}


Vector2f searchbar::mouse_world() {
    return registers::window->mapPixelToCoords(Mouse::getPosition(*registers::window));
}


bool searchbar::hit(const Vector2f & p) const {
    return this->shape.getGlobalBounds().contains(p);
}


void searchbar::handle_event(const Event & event) {
    if (event.is<Event::KeyPressed>()) {
        if (event.getIf<Event::KeyPressed>()->code == Keyboard::Key::Left) {
            if (this->begin_index-1) {
                if (this->begin_index == this->caret_index--) {
                    this->begin_index = this->caret_index;
                }
                this->caret_text.setPosition({this->text.getPosition().x + this->caret_index, this->text.getPosition().y});
            }
        }
        else if (event.getIf<Event::KeyPressed>()->code == Keyboard::Key::Right) {
            if (this->end_index+1 != this->field_size) {
                if (this->end_index == this->caret_index++) {
                    this->end_index = this->caret_index;
                }
                this->caret_text.setPosition({this->text.getPosition().x + this->caret_index, this->text.getPosition().y});
            }
        }
    }
    else if (const Vector2f mp = this->mouse_world(); this->hit(mp)) {
        if (const auto * mbe = event.getIf<Event::MouseButtonReleased>()) {
            if (mbe->button == Mouse::Button::Left) {
                this->activated = true;
            }
        }
    }
}


void searchbar::set_position(const Vector2f pos) {
    this->shape.setPosition(pos);

    this->text.setOrigin(this->text.getGlobalBounds().getCenter());
    const Vector2f c = this->shape.getTransform().transformPoint(this->shape.getLocalBounds().getCenter());
    this->text.setPosition(c + Vector2f(-330.f, 0.f));
}
