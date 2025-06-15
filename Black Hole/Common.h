#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <SFML/Graphics.hpp>

struct LightRay {
    double x = 0.0, y = 0.0;
    double vx = 0.0, vy = 0.0;
    bool active = true;
    std::vector<sf::Vector2f> history;
};

#endif