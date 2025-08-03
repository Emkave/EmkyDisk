#include "segment.h"

using namespace sf;
using namespace EDisk;


Segment::Segment(const std::filesystem::path && filename, const uintmax_t && depth) : depth(depth), filename(filename.wstring()) {
    this->setFillColor(Color::White);
    this->setRadius(60);
    this->setPosition({300, 200});
}


void Segment::update() {
    const Vector2i mouse_pos = Mouse::getPosition(*window);
    const Vector2f mouse_pos_f = {static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y)};

    if (this->getGlobalBounds().contains(mouse_pos_f)) {
        this->setFillColor(Color::Red);
    } else if (this->getFillColor() != Color::White) {
        this->setFillColor(Color::White);
    }
}