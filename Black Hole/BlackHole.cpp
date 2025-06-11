#include "BlackHole.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


BlackHole::BlackHole(double m, double x0, double y0, double z0) : mass(m), x(x0), y(y0), z(z0) {
    double Rs = 2 * G * mass / (c * c); // the radius of a sphere around a massive object, where the escape velocity equals the speed of light
}

bool BlackHole::isInsideEventHorizon(double px, double py, double pz) const {
    double dist = sqrt(pow(px - x, 2) + pow(py - y,2) + pow(pz - z,2));
    return dist < Rs;
}

void BlackHole::getAcceleration(double px, double py, double pz, double& ax, double& ay, double& az) const {
    double dist = sqrt(pow(px - x, 2) + pow(py - y,2) + pow(pz - z,2));
    if (dist < Rs) ax = ay = az = 0;
    else {
        ax = - G * mass * (px - x)/pow(dist, 3);
        ay = - G * mass * (py - y)/pow(dist, 3);
        az = - G * mass * (pz - z)/pow(dist, 3);
    }; 
}

double BlackHole::getDeflectionAngle(double b, double teta) const {
  if (b < Rs) teta = 0;
  else {
        teta = (4*G*mass)/pow(c,2)*b;
  }
}

double BlackHole::timeDialation(double px, double py, double pz) const {
    double dist = sqrt(pow(px - x, 2) + pow(py - y,2) + pow(pz - z,2));
    if (dist < Rs) return 0;
    else {
        return sqrt(1-Rs/dist);
    }
}

void updateParticle(const BlackHole& bh, Particle& p, double dt) {
    
}

void saveTrajectory(const std::vector<Particle>& particles, const std::string& filename) {}

void simulateAccretionDisk(const BlackHole& bh, std::vector<Particle>& particles, double dt, int steps) {};