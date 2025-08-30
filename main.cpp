#include <filesystem>
#include <iostream>
#include "disk.h"
#include "SFML/Graphics.hpp"
#include "vld.h"
#include "settings.h"
#include "window.h"

using namespace sf;

const float PI = 3.14159265f;


VertexArray createSector(const float radius, const float startAngle, const float endAngle, const int segments = 50) {
    VertexArray sector(PrimitiveType::TriangleFan);

    sector.append(Vertex(Vector2f(400.f, 300.f), Color::Green)); // window center

    const float angleStep = (endAngle - startAngle) / segments;
    for (int i = 0; i <= segments; ++i) {
        const float angle = startAngle + i * angleStep;
        const float x = 400.f + radius * cos(angle);
        const float y = 300.f + radius * sin(angle);
        sector.append(Vertex(Vector2f(x, y), Color::Green));
    }

    return sector;
}


int main() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 4;

    class window window;
    window.init();

    while (window.win().isOpen()) {
        while (const std::optional ev = window.win().pollEvent()) {
            if (ev->is<sf::Event::Closed>()) {
                window.win().close();
            }
            if (const auto * resized = ev->getIf<sf::Event::Resized>()) {
                sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
                window.win().setView(sf::View(visibleArea));
            }
        }

        window.win().clear(sf::Color(24, 24, 32));
        window.render();
        window.win().display();
    }

    window.win().close();



    return 0;
}