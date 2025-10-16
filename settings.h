#ifndef SETTINGS_H
#define SETTINGS_H
#include <SFML/Graphics.hpp>
#include <windows.h>
#define WIN_WIDTH 900
#define WIN_HEIGHT 80

using namespace sf;

namespace registers {
    constexpr float PI = 3.14159265f;
    inline RenderWindow * window = nullptr;
    inline std::wstring scan_path = L"C:/";
    inline Font font = Font("../arial.ttf");
    inline Clock clock;
}


#endif //SETTINGS_H
