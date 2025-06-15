#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include <SFML/Graphics.hpp>
#include "Common.h" // Include the common definitions

class BlackHole {
private:
    double mass;
    double x, y, z;
    static const double G;
    static const double SPEED_OF_LIGHT;

public:
    BlackHole(double m, double xPos, double yPos, double zPos);
    double getMass() const { return mass; }
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    double getEventHorizonRadius() const;
    void bendLightRay(LightRay& ray, double dt) const; // Added const qualifier
    static double getG() { return G; }                 // Public accessor for G
    static double getSpeedOfLight() { return SPEED_OF_LIGHT; } // Public accessor for SPEED_OF_LIGHT
};

#endif