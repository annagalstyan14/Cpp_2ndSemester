#include "Space.h"
#include "Settings.h"
#include <cmath>
#include <random>
#include <iostream>

Space::Space() {
    initStars(200);
}

void Space::initStars(int numStars) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(0, WINDOW_WIDTH);
    std::uniform_real_distribution<float> yDist(0, WINDOW_HEIGHT);

    stars.clear();
    for (int i = 0; i < numStars; ++i) {
        sf::Vertex star;
        star.position = sf::Vector2f(xDist(gen), yDist(gen));
        star.color = sf::Color::White;
        stars.push_back(star);
    }
}

std::vector<LightRay> Space::generateLightRays(int numRays, const BlackHole& bh) {
    std::vector<LightRay> rays;
    rays.reserve(numRays);

    double startX = 0; // Start left
    double startY = WINDOW_HEIGHT / 2.0;
    double spacing = 20.0; // Increased for wider spread

    for (int i = 0; i < numRays; ++i) {
        double x = startX;
        double y = startY + (i - numRays / 2) * spacing;

        // Rightward velocity (no tangential component)
        double speed = 100.0; // Match BlackHole.cpp
        double vx = speed;
        double vy = 0.0;

        LightRay ray;
        ray.x = x;
        ray.y = y;
        ray.vx = vx;
        ray.vy = vy;
        ray.active = true;
        ray.history.push_back(sf::Vector2f((float)x, (float)y));

        rays.push_back(ray);
    }

    return rays;
}

bool Space::animateRays(std::vector<LightRay>& rays, const BlackHole& bh, double dt) {
    bool anyActive = false;

    if (animationStep < MAX_STEPS) {
        for (auto& ray : rays) {
            if (ray.active) {
                bh.bendLightRay(ray, dt);
                ray.x += ray.vx * dt;
                ray.y += ray.vy * dt;

                ray.history.push_back(sf::Vector2f((float)ray.x, (float)ray.y));
                std::cout << "Ray pos: (" << ray.x << ", " << ray.y << "), vel: (" << ray.vx << ", " << ray.vy << "), active: " << ray.active << "\n";

                if (ray.active) {
                    anyActive = true;
                }
            }
        }
        animationStep++;
    }

    return anyActive;
}

void Space::renderStaticElements(sf::RenderWindow& window, const BlackHole& bh) {
    // Black hole shadow
    float coreRadius = (float)bh.getEventHorizonRadius() * 1.5f;
    sf::CircleShape blackHoleCore(coreRadius);
    blackHoleCore.setFillColor(sf::Color::Black);
    blackHoleCore.setOrigin(coreRadius, coreRadius);
    blackHoleCore.setPosition(bh.getX(), bh.getY());
    window.draw(blackHoleCore);

    // Accretion disk
    float accretionInnerRadius = coreRadius * 1.1f;
    float accretionOuterRadius = accretionInnerRadius * 1.2f;
    sf::CircleShape accretionDisk(accretionOuterRadius);
    accretionDisk.setFillColor(sf::Color::Transparent);
    accretionDisk.setOutlineThickness(accretionOuterRadius - accretionInnerRadius);
    accretionDisk.setOutlineColor(sf::Color::White);
    accretionDisk.setOrigin(accretionOuterRadius, accretionOuterRadius);
    accretionDisk.setPosition(bh.getX(), bh.getY());
    window.draw(accretionDisk);
}

void Space::renderLightRays(sf::RenderWindow& window, const std::vector<LightRay>& rays) {
    for (const auto& ray : rays) {
        if (ray.history.size() > 1) {
            sf::VertexArray trail(sf::LineStrip, ray.history.size());
            for (size_t i = 0; i < ray.history.size(); ++i) {
                trail[i].position = ray.history[i];
                trail[i].color = sf::Color::Red; // Solid red
            }
            window.draw(trail);
        }
    }
}

void Space::renderStars(sf::RenderWindow& window) {
    sf::VertexArray starArray(sf::Points, stars.size());
    for (size_t i = 0; i < stars.size(); ++i) {
        starArray[i] = stars[i];
    }
    window.draw(starArray);
}