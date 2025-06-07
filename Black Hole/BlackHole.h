#ifndef BLACK_HOLE_H
#define BLACK_HOLE_H

#include <vector>
#include <string>
#include <iostream>

const double G = 6.67430e-11;
const double c = 2.99792458e8;

struct Particle{
    double x, y, z;
    double vx, vy, vz;
    double mass;
};

class BlackHole{
    double x,y,z;
    double mass;
    double Rs;

    BlackHole(double x0 = 0, double y0 = 0, double z0 = 0, double m);
    bool BlackHole::isInsideEventHorizon(double px, double py, double pz) const;
    void BlackHole::getAcceleration(double px, double py, double pz, double& ax, double& ay, double& az) const;
    double BlackHole::getDeflectionAngle(double impactParameter) const;
};

void updateParticle();
void saveTrajectory();

#endif