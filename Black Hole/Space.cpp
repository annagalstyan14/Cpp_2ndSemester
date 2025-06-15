#include "Space.h"
#include "Settings.h" // For WINDOW_WIDTH and WINDOW_HEIGHT
#include <cmath>
#include <cstdlib>
#include <iostream> // For debug output
#define _USE_MATH_DEFINES

Space::Space() {}

std::vector<Particle> Space::generateAccretionDisk(const BlackHole& bh, int numParticles, double radius, double mass) {
    std::vector<Particle> particles;
    particles.reserve(numParticles);
    
    for (int i = 0; i < numParticles; ++i) {
        double angle = 2 * M_PI * i / numParticles;
        double r = radius * (0.5 + 0.5 * static_cast<double>(rand()) / RAND_MAX);
        
        double px = bh.getX() + r * cos(angle);
        double py = bh.getY() + r * sin(angle);
        double pz = bh.getZ();
        
        double v = std::sqrt(BlackHole::getG() * bh.getMass() / r);
        double vx = -v * sin(angle);
        double vy = v * cos(angle);
        double vz = 0;
        
        particles.push_back({px, py, pz, vx, vy, vz, mass, true});
    }
    
    return particles;
}

std::vector<LightRay> Space::generateLightRays(int numRays, const BlackHole& bh) {
    std::vector<LightRay> rays;
    for (int i = 0; i < numRays; ++i) {
        double startX = WINDOW_WIDTH * 0.75 + (WINDOW_WIDTH * 0.25 * i / (numRays - 1)); // Top-right quarter
        double startY = WINDOW_HEIGHT * 0.25 * i / (numRays - 1); // Spread vertically in top quarter
        double initialVx = -0.7; // Downward-left trajectory
        double initialVy = -0.7;
        LightRay ray = {startX, startY, initialVx, initialVy, false};
        ray.history.push_back(sf::Vector2f(static_cast<float>(startX), static_cast<float>(startY)));
        rays.push_back(ray);
    }
    return rays;
}

bool Space::animateRays(std::vector<LightRay>& rays, const BlackHole& bh, double dt) {
    bool anyActive = false;
    std::cout << "Animation step: " << animationStep << std::endl; // Debug output
    if (animationStep < MAX_STEPS) {
        int raysToActivate = std::min(5, static_cast<int>(rays.size()) - animationStep / 10);
        for (int i = 0; i < raysToActivate && animationStep / 10 + i < rays.size(); ++i) {
            if (!rays[animationStep / 10 + i].active) {
                rays[animationStep / 10 + i].active = true;
                std::cout << "Activated ray at (" << rays[animationStep / 10 + i].x << ", " << rays[animationStep / 10 + i].y << ")" << std::endl;
            }
        }
        for (auto& ray : rays) {
            if (ray.active) {
                bh.bendLightRay(ray, dt * 100); // Increase dt for more bending
                ray.x += ray.vx * dt;
                ray.y += ray.vy * dt;
                ray.history.push_back(sf::Vector2f(static_cast<float>(ray.x), static_cast<float>(ray.y)));
                if (ray.history.size() > 100) ray.history.erase(ray.history.begin()); // Limit history size
                std::cout << "Ray updated to (" << ray.x << ", " << ray.y << ") with vx=" << ray.vx << ", vy=" << ray.vy << std::endl;
                anyActive = true;
            }
        }
        animationStep++;
    }
    return anyActive;
}

void Space::renderStaticElements(sf::RenderWindow& window, const BlackHole& bh) {
    sf::Vector2u windowSize = window.getSize();
    float width = static_cast<float>(windowSize.x);
    float height = static_cast<float>(windowSize.y);

    // Black hole (event horizon)
    sf::CircleShape eventHorizon(25.0f); // Radius in pixels
    eventHorizon.setFillColor(sf::Color::Black);
    eventHorizon.setPosition(bh.getX() - 25.0f, bh.getY() - 25.0f); // Center it
    window.draw(eventHorizon);

    // Accretion disk (brown ring)
    sf::CircleShape accretionDisk(50.0f);
    accretionDisk.setFillColor(sf::Color(139, 69, 19)); // Brown
    accretionDisk.setPosition(bh.getX() - 50.0f, bh.getY() - 50.0f);
    accretionDisk.setOutlineThickness(5.0f);
    accretionDisk.setOutlineColor(sf::Color::Transparent);
    window.draw(accretionDisk);

    // Orange rings
    for (int i = 1; i <= 5; ++i) {
        sf::CircleShape ring(50.0f + i * 10.0f);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineThickness(2.0f);
        uint8_t intensity = static_cast<uint8_t>(200 * (1.0 - static_cast<float>(i) / 6));
        ring.setOutlineColor(sf::Color(255, 165, 0, intensity)); // Orange with fading
        ring.setPosition(bh.getX() - (50.0f + i * 10.0f), bh.getY() - (50.0f + i * 10.0f));
        window.draw(ring);
    }
}

void Space::renderLightRays(sf::RenderWindow& window, const std::vector<LightRay>& rays) {
    for (const auto& ray : rays) {
        if (ray.active && !ray.history.empty()) {
            sf::VertexArray trail(sf::LineStrip, ray.history.size());
            for (size_t j = 0; j < ray.history.size(); ++j) {
                trail[j].position = ray.history[j];
                trail[j].color = sf::Color(255, 69, 0, static_cast<uint8_t>(255 * (1.0 - static_cast<float>(j) / ray.history.size()))); // Fading red-orange
            }
            window.draw(trail);
        }
    }
}