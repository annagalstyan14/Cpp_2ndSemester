#include <SFML/Graphics.hpp>
#include "BlackHole.hpp"
#include "Photon.hpp"
#include "Vector2D.hpp"
#include <vector>

// We will use SIMULATION_DT defined in Photon.hpp and BlackHole.hpp
// const float c = 30.0f; // Not directly used here, but implicit in Photon velocity

int main() {
    int width = 1000;
    int height = 700;
    sf::RenderWindow window(sf::VideoMode(width, height), "Black Hole Visualization");

    Blackhole m87(width / 2, height / 2, 10000);
    float start = height / 2;
    float end = height / 2 - m87.rs * 2.6f;

    std::vector<Photon> particles;
    for (float y = 0; y < height; y += 10) {
        particles.emplace_back(width - 20, y);
    }

    // Draw background gradient once
    for (int i = 0; i < height; ++i) {
        sf::Color color1(40, 0, 80); // Darker purple at top (R, G, B)
        sf::Color color2(150, 0, 200); // Lighter purple at bottom
        float ratio = (float)i / height;
        sf::Color interpolatedColor(
            static_cast<sf::Uint8>(color1.r * (1 - ratio) + color2.r * ratio),
            static_cast<sf::Uint8>(color1.g * (1 - ratio) + color2.g * ratio),
            static_cast<sf::Uint8>(color1.b * (1 - ratio) + color2.b * ratio)
        );
        sf::RectangleShape line(sf::Vector2f(width, 1));
        line.setPosition(0, i);
        line.setFillColor(interpolatedColor);
        window.draw(line);
    }

    // Draw the two lines once
    sf::Vertex line1[] = {
        sf::Vertex(sf::Vector2f(0, start), sf::Color::Magenta),
        sf::Vertex(sf::Vector2f(width, start), sf::Color::Magenta)
    };
    sf::Vertex line2[] = {
        sf::Vertex(sf::Vector2f(0, end), sf::Color::Magenta),
        sf::Vertex(sf::Vector2f(width, end), sf::Color::Magenta)
    };
    window.draw(line1, 2, sf::Lines);
    window.draw(line2, 2, sf::Lines);

    // Draw black hole once
    m87.show(window);

    // Simulate photon paths for a fixed number of steps to create trails
    int simulationSteps = 500; // Adjust this number for longer/shorter trails
    for (int step = 0; step < simulationSteps; ++step) {
        for (auto& p : particles) {
            m87.pull(p);
            p.update();
            p.show(window); // Draw each photon at its current position
        }
    }

    window.display(); // Display the accumulated drawings

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
    return 0;
}
