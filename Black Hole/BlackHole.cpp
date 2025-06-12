#include "BlackHole.h"
#include <cmath>

BlackHole::BlackHole(double mass, float posX, float posY, float radius)
    : mass(mass), position(posX, posY), radius(radius) {}

sf::Vector2f BlackHole::getPosition() const {
    return position;
}

float BlackHole::getRadius() const {
    return radius;
}

double BlackHole::getMass() const {
    return mass;
}

double BlackHole::schwarzschildRadius() const {
    // Rs = 2GM/c^2 (meters)
    const double c = 2.99792458e8; // speed of light in m/s
    return 2 * G * mass / (c * c);
}

sf::Vector2f BlackHole::gravitationalAcceleration(const sf::Vector2f& pos) const {
    sf::Vector2f dir = position - pos;
    double distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (distance < 1e-1) // avoid division by zero or too close
        return sf::Vector2f(0, 0);

    double accelMagnitude = G * mass / (distance * distance);
    return dir / static_cast<float>(distance) * static_cast<float>(accelMagnitude);
}

void BlackHole::updateParticle(Particle& p, float deltaTime) {
    sf::Vector2f accel = gravitationalAcceleration(p.position);
    p.velocity += accel * deltaTime;
    p.position += p.velocity * deltaTime;

    // If particle falls into event horizon, reset or remove it (optional)
    sf::Vector2f diff = p.position - position;
    double dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if (dist < radius) {
        // For simplicity, respawn particle far away
        p.position = position + sf::Vector2f(100.f, 0.f);
        p.velocity = sf::Vector2f(0.f, 0.f);
    }
}

void BlackHole::bendLightRay(LightRay& ray, float deltaTime) {
    // Approximate gravitational lensing effect by bending ray direction slightly towards BH
    sf::Vector2f toBH = position - ray.position;
    double distance = std::sqrt(toBH.x * toBH.x + toBH.y * toBH.y);

    if (distance < 1e-1) return;

    // The bending angle ~ 4GM/(c^2 * b), b = impact parameter (distance)
    // We'll scale it down for visual effect
    const double c = 2.99792458e8;
    double bendingAngle = 4 * G * mass / (c * c * distance);

    // Calculate perpendicular direction to ray.direction towards BH
    sf::Vector2f dirNorm = ray.direction / std::sqrt(ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y);
    sf::Vector2f toBHNorm = toBH / static_cast<float>(distance);

    // Calculate cross product to determine bending side (+/-)
    float cross = dirNorm.x * toBHNorm.y - dirNorm.y * toBHNorm.x;
    float bendSign = (cross > 0) ? 1.f : -1.f;

    // Apply bending angle by rotating ray.direction slightly
    float cosA = std::cos(bendingAngle * bendSign);
    float sinA = std::sin(bendingAngle * bendSign);

    sf::Vector2f newDir;
    newDir.x = cosA * dirNorm.x - sinA * dirNorm.y;
    newDir.y = sinA * dirNorm.x + cosA * dirNorm.y;

    ray.direction = newDir;
    ray.position += ray.direction * deltaTime * 50.f; // Move ray forward scaled for visibility

    // Intensity decay over distance
    ray.intensity *= 0.995f;
}
