#ifndef SPACE_H
#define SPACE_H

#include "BlackHole.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Space {
public:
    Space();

    // Generate accretion disk particles around black hole
    std::vector<Particle> generateAccretionDisk(const BlackHole& bh, int count, float diskRadius, double scale);

    // Generate light rays starting from outside the BH, passing near BH
    std::vector<LightRay> generateLightRays(int count, const BlackHole& bh);

    // Render everything on SFML image
    void renderScene(sf::Image& image, const BlackHole& bh,
                     const std::vector<Particle>& particles,
                     const std::vector<LightRay>& rays);

private:
    void drawPixel(sf::Image& image, int x, int y, sf::Color color);

    void drawCircle(sf::Image& image, int cx, int cy, int radius, sf::Color color);

    // Optional: glow or distortion effect helpers
};

#endif
