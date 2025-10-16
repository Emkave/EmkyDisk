#include "searchbar.h"
#include "other.h"


Vector2f searchbar::mouse_world() {
    return registers::window->mapPixelToCoords(Mouse::getPosition(*registers::window));
}


bool searchbar::hit(const Vector2f & p) const {
    return this->shape.getGlobalBounds().contains(p);
}


searchbar::searchbar() {
    this->caret_text = Text(registers::font, this->caret_symbol);
    this->caret_text.setCharacterSize(17);
    this->caret_text.setPosition({30, 39});
    this->end_index = static_cast<short>(std::min(static_cast<size_t>(this->field_size-1), this->text.getString().getSize()-1));
    this->caret_index = this->end_index;
}


void searchbar::draw() const {
    registers::window->draw(this->shape);
    registers::window->draw(this->text);

    if (this->activated) {
        registers::window->draw(this->caret_text);
    }
}


void searchbar::update() {
    this->text.setString(registers::scan_path.substr(this->begin_index, this->end_index+1));
}


void searchbar::reindex() {
    this->end_index = static_cast<short>(std::min(static_cast<size_t>(this->field_size-1), this->text.getString().getSize()-1));
    this->update();
}


void searchbar::handle_event(const Event & event) {
    if (this->activated) {
        if (event.is<Event::KeyPressed>()) {
            if (event.getIf<Event::KeyPressed>()->code == Keyboard::Key::Left) {
                if (this->caret_index != this->begin_index) {
                    this->caret_index--;
                    this->caret_text.setPosition({this->caret_text.getPosition().x-this->field_size/this->text.getCharacterSize()-5, this->caret_text.getPosition().y});
                } else {
                    if (this->begin_index) {
                        this->begin_index = this->caret_index -= 1;
                    } else if (this->end_index - this->begin_index >= this->field_size) {
                        this->end_index--;
                    }
                }
            }
            else if (event.getIf<Event::KeyPressed>()->code == Keyboard::Key::Right) {
                if (this->caret_index != this->end_index) {
                    this->caret_index++;
                    this->caret_text.setPosition({this->caret_text.getPosition().x+this->field_size/this->text.getCharacterSize()+5, this->caret_text.getPosition().y});
                } else {
                    if (this->end_index < this->text.getString().getSize()) {
                        this->end_index = this->caret_index += 1;
                    } else if (this->end_index > this->field_size) {
                        this->begin_index++;
                    }
                }
            }
        }

        this->update();
    }

    if (const Vector2f mp = searchbar::mouse_world(); this->hit(mp)) {
        if (const auto * mbe = event.getIf<Event::MouseButtonReleased>()) {
            if (mbe->button == Mouse::Button::Left) {
                this->activated = true;
            }
        }
    }
    else if (const auto * kbe = event.getIf<Event::KeyPressed>()) {
        if (kbe->code == Keyboard::Key::Escape) {
            this->activated = false;
        }
    }
}


void searchbar::set_position(const Vector2f pos) {
    this->shape.setPosition(pos);

    this->text.setOrigin(this->text.getGlobalBounds().getCenter());
    const Vector2f c = this->shape.getTransform().transformPoint(this->shape.getLocalBounds().getCenter());
    this->text.setPosition(c + Vector2f(-330.f, 0.f));
}
