#ifndef SPACE_H
#define SPACE_H

#include <SFML/Graphics.hpp>
#include "Common.h"
#include "BlackHole.h"
#include <vector>

class Space {
public:
    Space();
    void initStars(int numStars);
    std::vector<LightRay> generateLightRays(int numRays, const BlackHole& bh);
    bool animateRays(std::vector<LightRay>& rays, const BlackHole& bh, double dt);
    void renderStaticElements(sf::RenderWindow& window, const BlackHole& bh);
    void renderLightRays(sf::RenderWindow& window, const std::vector<LightRay>& rays);
    void renderStars(sf::RenderWindow& window);

    int animationStep = 0;
    static constexpr int MAX_STEPS = 5000;

private:
    std::vector<sf::Vertex> stars;
};

#endif