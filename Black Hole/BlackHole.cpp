#include "BlackHole.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


BlackHole::BlackHole(double m, double x0, double y0, double z0) : mass(m), x(x0), y(y0), z(z0) {
    if(m <= 0) throw std::invalid_argument("Mass must be positive");
    rs = 2 * G * mass / (c * c); // the radius of a sphere around a massive object, where the escape velocity equals the speed of light
}

bool BlackHole::isInsideEventHorizon(double px, double py, double pz) const {
    double dist = sqrt(pow(px - x, 2) + pow(py - y,2) + pow(pz - z,2));
    return dist < rs;
}

void BlackHole::getAcceleration(double px, double py, double pz, double& ax, double& ay, double& az) const {
    double dist = sqrt(pow(px - x, 2) + pow(py - y,2) + pow(pz - z,2));
    if (dist < rs) ax = ay = az = 0;
    else {
        ax = - G * mass * (px - x)/pow(dist, 3);
        ay = - G * mass * (py - y)/pow(dist, 3);
        az = - G * mass * (pz - z)/pow(dist, 3);
    }; 
}

double BlackHole::getDeflectionAngle(double b) const {
  if (b < rs) return 0.0;
        return (4*G*mass)/pow(c,2)*b;  
}

double BlackHole::timeDialation(double px, double py, double pz) const {
    double dist = sqrt(pow(px - x, 2) + pow(py - y,2) + pow(pz - z,2));
    if (dist < rs) return 0.0;
    else {
        return sqrt(1-rs/dist);
    }
}

void updateParticle(const BlackHole& bh, Particle& p, double dt) {
    if(bh.isInsideEventHorizon(p.x, p.y, p.z)){
        return;
    }

    double ax, ay, az;
    bh.getAcceleration(p.x,p.y,p.z,ax,ay,az);

    p.vx += ax *dt;
    p.vy += ay *dt;
    p.vz += az *dt;

    p.x += p.vx * dt;
    p.y += p.vy * dt;
    p.z += p.vz * dt;
}

void saveTrajectory(const std::vector<Particle>& particles, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()){
        throw std::runtime_error("Cannot open file: " + filename);
    }
    file << "x,y,z\n";

    for(const auto& p : particles){
        file << p.x << "," << p.y << "," << p.z << "\n";
    }
    file.close();
}

//void simulateAccretionDisk(const BlackHole& bh, std::vector<Particle>& particles, double dt, int steps) {};