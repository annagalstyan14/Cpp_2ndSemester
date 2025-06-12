#include <SFML/Graphics.hpp>
#include "BlackHole.h"
#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    sf::RenderWindow window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "Black Hole Simulation");
    window.setFramerateLimit(60);

    BlackHole blackHole(50.0f, sf::Vector2f(SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::White); // Clear with white background
        blackHole.draw(window);
        window.display();
    }

    return 0;
}
