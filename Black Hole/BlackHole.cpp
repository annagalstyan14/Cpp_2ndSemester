#include "BlackHole.h"
#include <cmath>
#include <iostream>

BlackHole::BlackHole(double m, double xPos, double yPos, double zPos) : mass(m), x(xPos), y(yPos), z(zPos) {}

double BlackHole::getEventHorizonRadius() const {
    double radius = 2.0 * G * mass / (SPEED_OF_LIGHT * SPEED_OF_LIGHT);
    std::cout << "Event Horizon Radius: " << radius << "\n";
    return radius;
}

void BlackHole::bendLightRay(LightRay& ray, double dt) const {
    double dx = x - ray.x;
    double dy = y - ray.y;
    double r = std::sqrt(dx * dx + dy * dy);

    if (r <= getEventHorizonRadius()) {
        ray.active = false; // Photon swallowed by black hole
        return;
    }

    if (r > 0) {
        // Gravitational deflection: 4GM/r, tuned for late bending
        double deflection = 4.0 * G * mass / (r * SPEED_OF_LIGHT * SPEED_OF_LIGHT);
        double scale_factor = 100; // Kept low to minimize early bending
        double accel = deflection * SPEED_OF_LIGHT * scale_factor;

        // Normalize direction to black hole
        double r_inv = 1.0 / r;
        double nx = dx * r_inv;
        double ny = dy * r_inv;

        // Apply acceleration perpendicular to velocity
        double v_magnitude = std::sqrt(ray.vx * ray.vx + ray.vy * ray.vy);
        if (v_magnitude == 0) return;

        double vx_norm = ray.vx / v_magnitude;
        double vy_norm = ray.vy / v_magnitude;
        double dot = nx * vx_norm + ny * vy_norm;
        double perp_nx = nx - dot * vx_norm;
        double perp_ny = ny - dot * vy_norm;

        double perp_magnitude = std::sqrt(perp_nx * perp_nx + perp_ny * perp_ny);
        if (perp_magnitude > 0) {
            perp_nx /= perp_magnitude;
            perp_ny /= perp_magnitude;
            ray.vx += accel * perp_nx * dt;
            ray.vy += accel * perp_ny * dt;
        }

        // Renormalize velocity
        v_magnitude = std::sqrt(ray.vx * ray.vx + ray.vy * ray.vy);
        if (v_magnitude > 0) {
            ray.vx = ray.vx / v_magnitude * 50.0;
            ray.vy = ray.vy / v_magnitude * 50.0;
        }
    }
}