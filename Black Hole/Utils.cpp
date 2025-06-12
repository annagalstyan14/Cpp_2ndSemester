// Utils.cpp
#include "Utils.h"
#include <fstream>
#include <iomanip>
#include <sstream>

// Linear interpolate between two colors
sf::Color interpolateColor(const sf::Color& start, const sf::Color& end, float t) {
    t = std::max(0.f, std::min(1.f, t));  // clamp t to [0,1]
    return sf::Color(
        static_cast<sf::Uint8>(start.r + t * (end.r - start.r)),
        static_cast<sf::Uint8>(start.g + t * (end.g - start.g)),
        static_cast<sf::Uint8>(start.b + t * (end.b - start.b)),
        static_cast<sf::Uint8>(start.a + t * (end.a - start.a))
    );
}

// Save particle positions to CSV file
void saveTrajectory(const std::vector<Particle>& particles, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << "x,y,vx,vy\n";
    for (const auto& p : particles) {
        file << p.x << "," << p.y << "," << p.vx << "," << p.vy << "\n";
    }
    file.close();
}

// Format double with fixed precision to string
std::string formatDouble(double value, int precision) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}
