#ifndef BLACKHOLE_HPP
#define BLACKHOLE_HPP
#include <SFML/Graphics.hpp>
#include "Vector2D.hpp"
#include "Photon.hpp"

constexpr float G = 3.54f; // Gravitational constant (JS version)
// constexpr float BH_DT = 0.1f;   // This will be replaced by SIMULATION_DT from Photon.hpp

class Blackhole {
public:
    Vector2D pos;
    float mass;
    float rs;
    
    Blackhole(float x, float y, float m);
    void pull(Photon& photon);
    void show(sf::RenderWindow& window);
};

Blackhole::Blackhole(float x, float y, float m) : pos(x, y), mass(m) {
    rs = 2 * G * mass / (30.0f * 30.0f);  // Schwarzschild radius, c = 30.0f
}

void Blackhole::pull(Photon& photon) {
    Vector2D dir = pos - photon.pos;
    float r = dir.magnitude();
    if (r < rs) return;  // Photon absorbed
    
    float force = G * mass / (r * r);
    Vector2D acc = dir * (force / r);
    // Use SIMULATION_DT from Photon.hpp
    photon.vel = photon.vel + acc * SIMULATION_DT;
}

void Blackhole::show(sf::RenderWindow& window) {
    // Event Horizon (Black Hole)
    sf::CircleShape eventHorizon(rs);
    eventHorizon.setFillColor(sf::Color::Black);
    eventHorizon.setPosition(pos.x - rs, pos.y - rs);
    window.draw(eventHorizon);
    
    // Photon Sphere (transparent with black outline)
    sf::CircleShape photonSphere(rs * 2.6f);
    photonSphere.setFillColor(sf::Color::Transparent);
    photonSphere.setOutlineColor(sf::Color::Black);
    photonSphere.setOutlineThickness(1);
    photonSphere.setPosition(pos.x - rs * 2.6f, pos.y - rs * 2.6f);
    window.draw(photonSphere);
}

#endif