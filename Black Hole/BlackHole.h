#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include <SFML/Graphics.hpp>
#include "Common.h"

class BlackHole {
private:
    double mass;
    double x, y, z;
    static constexpr double G = 1.0;
    static constexpr double SPEED_OF_LIGHT = 1.0;

public:
    BlackHole(double m, double xPos, double yPos, double zPos);
    double getMass() const { return mass; }
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    double getEventHorizonRadius() const;
    void bendLightRay(LightRay& ray, double dt) const;
};
#endif