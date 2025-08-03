#include <filesystem>
#include <iostream>

#include "disk.h"
#include "SFML/Graphics.hpp"
#include "vld.h"
#include "settings.h"

using namespace sf;

const float PI = 3.14159265f;


// VertexArray createSector(DiskUsage & dsk, const float radius, const float startAngle, const float endAngle, const int segments = 50) {
//     VertexArray sector(PrimitiveType::TriangleFan);
//
//     sector.append(Vertex(Vector2f(400.f, 300.f), Color::Green)); // window center
//
//     const float angleStep = (endAngle - startAngle) / segments;
//     for (int i = 0; i <= segments; ++i) {
//         const float angle = startAngle + i * angleStep;
//         const float x = 400.f + radius * cos(angle);
//         const float y = 300.f + radius * sin(angle);
//         sector.append(Vertex(Vector2f(x, y), Color::Green));
//     }
//
//     return sector;
// }




int main() {
    EDisk::Disk disk;
    const std::string path = "C:/Users/rcybe/Desktop/Архивы";
    std::cout << path << std::endl;

    //disk.set_path(path);
    //disk.scan();



    return 0;

    ContextSettings settings;
    settings.antiAliasingLevel = 4;

    //RenderWindow window (VideoMode({1280, 720}), "EmkyDisk", Style::Default, State::Windowed, settings);
    window = new RenderWindow(VideoMode({1280, 720}), "EmkyDisk", Style::Close, State::Windowed, settings);
    window->setFramerateLimit(120);

    // float radius = 100.f;
    // float currentAngle = PI / 2.f;        // 90 degrees (starting size)
    // float currentHalfAngle = 0.f;
    // float targetHalfAngle = (2.f * PI) * disk_usage.used_percent / 2.f;
    //
    // float speed = PI / 2.f; // radians per second (e.g. 90° per second)


    Clock clock;
    //EDisk::Disk disk ("D:/");


    while (window->isOpen()) {
        while (const std::optional event = window->pollEvent()) {
            if (event->is<Event::Closed>()) {
                window->close();
            }
        }

        const float deltaTime = clock.restart().asSeconds();

        /*if (currentHalfAngle < targetHalfAngle) {
            currentHalfAngle += speed * deltaTime;
            if (currentHalfAngle > targetHalfAngle)
                currentHalfAngle = targetHalfAngle;
        }


        // Rebuild sector
        VertexArray sector = createSector(disk_usage, radius, -currentHalfAngle, currentHalfAngle);*/

        window->clear(Color(52, 38, 86));
        //window->draw(sector);

        // for (EDisk::Segment & segment : disk.get_depth_segments()) {
        //     segment.update();
        //     window->draw(segment);
        // }

        window->display();
    }

    delete window;

    return 0;
}