#include "BlackHole.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <fstream>

int main() {
    // SFML setup
    sf::RenderWindow window(sf::VideoMode(512, 512), "Black Hole");
    sf::Image image;
    image.create(512, 512, sf::Color::White);
    sf::Texture texture;
    if (!texture.loadFromImage(image)) return 1;
    sf::Sprite sprite(texture);

    // Simulation setup
    BlackHole bh(1.989e30, 0, 0, 0);
    sf::CircleShape blackHoleShape(50.f); // Radius in pixels
    blackHoleShape.setFillColor(sf::Color::Black);
    blackHoleShape.setOrigin(50.f, 50.f); // Set origin to center for positioning
    blackHoleShape.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f); // Center of the window

    std::vector<Particle> particles = {
        {1e9, 0, 0, 0, 1e6, 0} // Example particle: r=1e9 m, vy=1e6 m/s
    };
    int step = 0;
    const int maxSteps = 1000;

    // Main loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Simulation (one step per frame)
        if (step < maxSteps) {
            for (auto& p : particles) {
                updateParticle(p, bh, 0.01);
            }
            saveTrajectory(particles, "traj_" + std::to_string(step) + ".csv");
            step++;
        }

        // Render
        window.clear(sf::Color::White);
        window.draw(blackHoleShape); // Draw black hole
        window.display();
    }

    return 0;
}