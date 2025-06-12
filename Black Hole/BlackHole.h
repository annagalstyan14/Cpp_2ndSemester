#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include <SFML/Graphics.hpp>
#include <vector>

// Forward declare particle and ray structs
struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;
};

struct LightRay {
    sf::Vector2f position;
    sf::Vector2f direction;
    float intensity;
};

class BlackHole {
public:
    BlackHole(double mass, float posX, float posY, float radius);

    sf::Vector2f getPosition() const;
    float getRadius() const;
    double getMass() const;

    void updateParticle(Particle& p, float deltaTime);
    void bendLightRay(LightRay& ray, float deltaTime);

private:
    double mass; // in kg
    sf::Vector2f position;
    float radius; // event horizon radius in pixels (scaled)

    const double G = 6.67430e-11; // gravitational constant

    double schwarzschildRadius() const;
    sf::Vector2f gravitationalAcceleration(const sf::Vector2f& pos) const;
};

#endif
