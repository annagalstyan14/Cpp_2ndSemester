#include "BlackHole.h"

BlackHole::BlackHole(float radius, const sf::Vector2f& position) {
    blackHoleShape.setRadius(radius);
    blackHoleShape.setOrigin(radius, radius); // Set origin to center for easier positioning
    blackHoleShape.setPosition(position);
    blackHoleShape.setFillColor(sf::Color::Black);
}

void BlackHole::draw(sf::RenderWindow& window) {
    window.draw(blackHoleShape);
} 