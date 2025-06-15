#ifndef SPACE_H
#define SPACE_H

#include <SFML/Graphics.hpp>
#include "Common.h" // Include the common definitions
#include "BlackHole.h"

class Space {
public:
    Space();
    std::vector<Particle> generateAccretionDisk(const BlackHole& bh, int numParticles, double radius, double mass);
    std::vector<LightRay> generateLightRays(int numRays, const BlackHole& bh);
    bool animateRays(std::vector<LightRay>& rays, const BlackHole& bh, double dt);
    void renderStaticElements(sf::RenderWindow& window, const BlackHole& bh);
    void renderLightRays(sf::RenderWindow& window, const std::vector<LightRay>& rays);
    int animationStep = 0;
    static constexpr int MAX_STEPS = 1000;

};

#endif