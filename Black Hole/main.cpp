#include <SFML/Graphics.hpp>
#include "BlackHole.h"
#include "Space.h"
#include "Settings.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Black Hole Simulation");
    window.setFramerateLimit(FPS_LIMIT);

    BlackHole bh(10.0, WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f, 0.0); // Increased mass
    Space space;
    auto rays = space.generateLightRays(50, bh); // Increased to 50 rays

    bool isRunning = true;
    bool isSimulating = false;

    while (isRunning) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                isRunning = false;
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    isRunning = false;
                if (event.key.code == sf::Keyboard::Space) {
                    isSimulating = !isSimulating;
                    if (isSimulating) {
                        space.animationStep = 0;
                        rays = space.generateLightRays(50, bh);
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        space.renderStars(window);
        space.renderStaticElements(window, bh);

        if (isSimulating) {
            if (!space.animateRays(rays, bh, 0.075)) { // Small dt for smooth paths
                isSimulating = false;
            }
            space.renderLightRays(window, rays);
        }

        window.display();
    }

    return 0;
}