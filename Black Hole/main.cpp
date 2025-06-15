#include <SFML/Graphics.hpp>
#include "BlackHole.h"
#include "Space.h"
#include "Settings.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Black Hole Visualization");
    window.setFramerateLimit(FPS_LIMIT);

    BlackHole bh(10.0, WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0, 0.0); // Center of screen
    Space space;
    auto rays = space.generateLightRays(500, bh);

    bool isSimulationRunning = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                std::cout << "Key pressed: " << event.key.code << std::endl;
                switch (event.key.code) {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::Space:
                        isSimulationRunning = !isSimulationRunning;
                        if (isSimulationRunning) {
                            space.animationStep = 0;
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        window.clear(sf::Color::White); // White background

        // Render static elements (black hole and accretion disk)
        space.renderStaticElements(window, bh);

        // Render and animate dynamic elements (photon trails) if simulation is running
        if (isSimulationRunning) {
            std::cout << "Animating frame, step: " << space.animationStep << std::endl;
            if (space.animateRays(rays, bh, 0.008)) { // dt = 1/60s for 60 FPS
                space.renderLightRays(window, rays);
            } else {
                isSimulationRunning = false;
            }
        }

        window.display();
    }

    return 0;
}