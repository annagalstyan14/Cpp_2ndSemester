#ifndef BLACK_HOLE_H
#define BLACK_HOLE_H

#include <vector>
#include <string>

const double G = 6.67430e-11;
const double c = 2.99792458e8;

struct Particle{
    double x, y, z;
    double vx, vy, vz;
    double mass;
};

class BlackHole{

};

void updateParticle();
void saveTrajectory();

#endif