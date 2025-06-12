#ifndef BLACK_HOLE_H
#define BLACK_HOLE_H

#include <SFML/Graphics.hpp>

class BlackHole {
public:
    BlackHole(float radius, const sf::Vector2f& position);
    void draw(sf::RenderWindow& window);

private:
    sf::CircleShape blackHoleShape;
};

#endif // BLACK_HOLE_H 