// Utils.h
#ifndef UTILS_H
#define UTILS_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

sf::Color interpolateColor(const sf::Color& start, const sf::Color& end, float t);
void saveTrajectory(const std::vector<Particle>& particles, const std::string& filename);
std::string formatDouble(double value, int precision = 2);

#endif
