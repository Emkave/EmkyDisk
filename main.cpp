#include "SFML/Graphics.hpp"
#include "vld.h"

using namespace sf;

const float PI = 3.14159265f;

VertexArray createSector(float radius, float startAngle, float endAngle, int segments = 50) {
    VertexArray sector(sf::TriangleFan);

    sector.append(sf::Vertex(sf::Vector2f(400.f, 300.f), sf::Color::Green)); // window center

    float angleStep = (endAngle - startAngle) / segments;
    for (int i = 0; i <= segments; ++i) {
        float angle = startAngle + i * angleStep;
        float x = 400.f + radius * cos(angle);
        float y = 300.f + radius * sin(angle);
        sector.append(Vertex(sf::Vector2f(x, y), sf::Color::Green));
    }

    return sector;
}


int main() {
    ContextSettings settings;
    settings.antialiasingLevel = 8;

    RenderWindow window (VideoMode(1280, 720), "EmkyDisk", Style::Default, settings);
    window.setFramerateLimit(120);

    float radius = 100.f;
    float currentAngle = PI / 2.f;        // 90 degrees (starting size)
    float targetAngle = 2.f * PI / 2.f;   // 270 degrees
    float speed = PI / 2.f; // radians per second (e.g. 90° per second)


    Clock clock;


    while (window.isOpen()) {
        Event event {};

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        const float deltaTime = clock.restart().asSeconds();

        if (currentAngle < targetAngle) {
            currentAngle += speed * deltaTime;
            if (currentAngle > targetAngle)
                currentAngle = targetAngle;
        }

        // Rebuild sector
        sf::VertexArray sector = createSector(radius, 0.f, currentAngle);

        window.clear();
        window.draw(sector);
        window.display();
    }

    return 0;
}