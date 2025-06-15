#ifndef PHOTON_HPP
#define PHOTON_HPP
#include <SFML/Graphics.hpp>
#include "Vector2D.hpp"

constexpr float SIMULATION_DT = 0.1f; // Global time step (JS version)

class Photon {
public:
    Vector2D pos;
    Vector2D vel;
    
    Photon(float x, float y);
    void update();
    void show(sf::RenderWindow& window);
};

Photon::Photon(float x, float y) : pos(x, y), vel(-30.0f, 0) {}  // Photons move left at speed c

void Photon::update() {
    pos = pos + vel * SIMULATION_DT;
}

void Photon::show(sf::RenderWindow& window) {
    sf::CircleShape circle(1); // Smaller circle for trails
    circle.setFillColor(sf::Color::Magenta);
    circle.setPosition(pos.x - 0.5f, pos.y - 0.5f); // Adjust position for smaller radius
    window.draw(circle);
}

#endif