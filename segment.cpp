#include "segment.h"
#include <iostream>
#include <utility>

using namespace sf;
using namespace EDisk;


Segment::Segment(std::filesystem::path path, size_t size, uintmax_t depth) : depth(depth), size(size), path(std::move(path)) {
    // this->setFillColor(Color::White);
    // this->setRadius(60);
    // this->setPosition({300, 200});
}


void Segment::update() {
    const Vector2i mouse_pos = Mouse::getPosition(*registers::window);
    const Vector2f mouse_pos_f = {static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y)};

    // if (this->getGlobalBounds().contains(mouse_pos_f)) {
    //     this->setFillColor(Color::Red);
    // } else if (this->getFillColor() != Color::White) {
    //     this->setFillColor(Color::White);
    // }
}


std::filesystem::path Segment::construct_path(void) const {
    std::wstring str = this->path.wstring();

    for (const Segment * ptr = this->parent; ptr; ptr = ptr->parent) {
        str = ptr->path.wstring() + "/" + str;
    }

    if (!std::filesystem::exists(str)) {
        std::wcerr << "Segment::construct_path() -> invalid path constructed as: " << str << std::endl;
        return "";
    }

    return str;
}
