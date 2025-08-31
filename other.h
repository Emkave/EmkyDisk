#ifndef OTHER_H
#define OTHER_H
#include <SFML/Graphics.hpp>

using namespace sf;

namespace functions {
    ConvexShape make_round_rect(const Vector2f, float, const int);

    void reset_view_to_window();
}


#endif //OTHER_H
