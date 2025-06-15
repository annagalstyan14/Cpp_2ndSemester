#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP
#include <cmath>

struct Vector2D {
    float x, y;
    Vector2D(float x_ = 0, float y_ = 0) : x(x_), y(y_) {}
    
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }
    
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }
    
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }
    
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }
};

#endif