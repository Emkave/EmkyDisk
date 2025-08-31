#ifndef SETTINGS_H
#define SETTINGS_H
#include <SFML/Graphics.hpp>
#define WIN_WIDTH 900
#define WIN_HEIGHT 80

using namespace sf;

namespace registers {
    inline RenderWindow * window = nullptr;
    inline std::string init_scan_path = "C:/";
    inline Font font = Font("../arial.ttf");
    inline Clock clock;
}


#endif //SETTINGS_H
