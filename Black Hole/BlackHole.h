#ifndef BLACK_HOLE_H
#define BLACK_HOLE_H

#include <vector>
#include <string>

const double G = 6.67430e-11;
const double c = 2.99792458e8;

struct Particle {
    double x, y, z;
    double vx, vy, vz;
};

class BlackHole {
private:
    double x, y, z;
    double mass;
    double rs;

public:
    BlackHole(double m, double x0, double y0, double z0);
    bool isInsideEventHorizon(double px, double py, double pz) const;
    void getAcceleration(double px, double py, double pz, double& ax, double& ay, double& az) const;
    double getDeflectionAngle(double b) const;
    double computeTimeDilation(double px, double py, double pz) const;
};

void updateParticle(Particle& p, const BlackHole& bh, double dt);
void saveTrajectory(const std::vector<Particle>& particles, const std::string& filename);

#endif