#include "BlackHole.h"
#include <cmath>

const double BlackHole::G = 1.0;
const double BlackHole::SPEED_OF_LIGHT = 1.0;


BlackHole::BlackHole(double m, double xPos, double yPos, double zPos) : mass(m), x(xPos), y(yPos), z(zPos) {}

double BlackHole::getEventHorizonRadius() const {
    return 2.0 * G * mass / (SPEED_OF_LIGHT * SPEED_OF_LIGHT); // Schwarzschild radius
}

void BlackHole::bendLightRay(LightRay& ray, double dt) const {
    double dx = ray.x - x;
    double dy = ray.y - y;
    double r = std::sqrt(dx * dx + dy * dy);
    if (r > 0 && r > getEventHorizonRadius()) { // Avoid singularity
        double force = G * mass / (r * r) * 1e8; // Increased force for visible bending
        double ax = -force * dx / r;
        double ay = -force * dy / r;
        ray.vx += ax * dt;
        ray.vy += ay * dt;
        // ray.x += ray.vx * dt; // Moved to animateRays to control step size
        // ray.y += ray.vy * dt; // Moved to animateRays to control step size
    }
}