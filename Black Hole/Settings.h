#ifndef SETTINGS_H
#define SETTINGS_H

// Window dimensions
constexpr int WINDOW_WIDTH = 512;
constexpr int WINDOW_HEIGHT = 512;

// Simulation settings
constexpr int NUM_PARTICLES = 500;         // Number of particles in accretion disk
constexpr int NUM_LIGHT_RAYS = 200;        // Number of light rays for lensing effect

// Black Hole settings
constexpr double BLACK_HOLE_MASS = 1.989e30; // Mass of sun approx, can tweak for size
constexpr float BLACK_HOLE_RADIUS = 10.f;    // Event horizon radius in pixels (scaled for visibility)

// Simulation timing
constexpr float TIME_STEP = 0.01f;           // Delta time for simulation step

#endif
