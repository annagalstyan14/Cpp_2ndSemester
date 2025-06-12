#include "Space.h"
#include <cmath>
#include <cstdlib>

Space::Space() {}

std::vector<Particle> Space::generateAccretionDisk(const BlackHole& bh, int count, float diskRadius, double scale) {
    std::vector<Particle> particles;
    particles.reserve(count);

    sf::Vector2f center = bh.getPosition();

    for (int i = 0; i < count; ++i) {
        // Generate random radius within diskRadius
        float r = static_cast<float>(std::rand()) / RAND_MAX * diskRadius * 0.5f + diskRadius * 0.5f; // from 0.5*R to R

        // Random angle around black hole
        float angle = static_cast<float>(std::rand()) / RAND_MAX * 2 * 3.14159265f;

        // Position in disk plane
        float x = center.x + r * std::cos(angle);
        float y = center.y + r * std::sin(angle);

        Particle p;
        p.position = sf::Vector2f(x, y);
        p.mass = 1e5f;  // arbitrary small mass

        // Circular orbit velocity magnitude (Newtonian)
        // v = sqrt(GM/r)
        double G = 6.67430e-11;
        double M = bh.getRadius() > 0 ? bh.getRadius() * 1e30 : 1.989e30; // mass guess (make sure bh mass accessible if needed)
        double v = std::sqrt(G * 1.989e30 / (r * scale));

        // Velocity vector perpendicular to radius vector (tangential)
        float vx = -v * std::sin(angle);
        float vy = v * std::cos(angle);

        // Scale down velocity to pixel units (adjust experimentally)
        p.velocity = sf::Vector2f(static_cast<float>(vx * 1e-3), static_cast<float>(vy * 1e-3));

        particles.push_back(p);
    }
    return particles;
}

std::vector<LightRay> Space::generateLightRays(int count, const BlackHole& bh) {
    std::vector<LightRay> rays;
    rays.reserve(count);

    sf::Vector2f center = bh.getPosition();

    // Generate rays starting from left of screen moving right (can adjust for lensing)
    for (int i = 0; i < count; ++i) {
        LightRay ray;

        float y = static_cast<float>(i) / count * 512; // evenly spaced vertically

        ray.position = sf::Vector2f(0, y);
        ray.direction = sf::Vector2f(1.f, 0.f); // moving right
        ray.intensity = 1.f;

        rays.push_back(ray);
    }
    return rays;
}

void Space::drawPixel(sf::Image& image, int x, int y, sf::Color color) {
    if (x >= 0 && x < 512 && y >= 0 && y < 512) {
        sf::Color existing = image.getPixel(x, y);

        // Blend colors (simple additive for glow)
        sf::Uint8 r = std::min(255, existing.r + color.r);
        sf::Uint8 g = std::min(255, existing.g + color.g);
        sf::Uint8 b = std::min(255, existing.b + color.b);

        image.setPixel(x, y, sf::Color(r, g, b));
    }
}

void Space::drawCircle(sf::Image& image, int cx, int cy, int radius, sf::Color color) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        drawPixel(image, cx + x, cy + y, color);
        drawPixel(image, cx + y, cy + x, color);
        drawPixel(image, cx - y, cy + x, color);
        drawPixel(image, cx - x, cy + y, color);
        drawPixel(image, cx - x, cy - y, color);
        drawPixel(image, cx - y, cy - x, color);
        drawPixel(image, cx + y, cy - x, color);
        drawPixel(image, cx + x, cy - y, color);

        if (err <= 0) {
            ++y;
            err += 2 * y + 1;
        }
        if (err > 0) {
            --x;
            err -= 2 * x + 1;
        }
    }
}

void Space::renderScene(sf::Image& image, const BlackHole& bh,
                        const std::vector<Particle>& particles,
                        const std::vector<LightRay>& rays) {
    // Clear to black first
    for (int y = 0; y < 512; ++y) {
        for (int x = 0; x < 512; ++x) {
            image.setPixel(x, y, sf::Color::Black);
        }
    }

    // Draw black hole event horizon (solid circle)
    drawCircle(image, static_cast<int>(bh.getPosition().x), static_cast<int>(bh.getPosition().y),
               static_cast<int>(bh.getRadius()), sf::Color(20, 20, 20));

    // Draw accretion disk particles (bright white)
    for (const auto& p : particles) {
        int px = static_cast<int>(p.position.x);
        int py = static_cast<int>(p.position.y);
        drawPixel(image, px, py, sf::Color::White);
    }

    // Draw light rays (bright yellow with intensity)
    for (const auto& ray : rays) {
        if (ray.intensity > 0.05f) {
            int px = static_cast<int>(ray.position.x);
            int py = static_cast<int>(ray.position.y);
            sf::Uint8 brightness = static_cast<sf::Uint8>(255 * ray.intensity);
            drawPixel(image, px, py, sf::Color(brightness, brightness, 0));
        }
    }
}
