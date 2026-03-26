#include <cmath>

class Vec2 {

public:
    double x;
    double y;

    Vec2(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y);}
    Vec2 operator*(double scalar)     const { return Vec2(x * scalar, y * scalar);}

    double length()   const { return std::sqrt(x * x + y * y);}
    Vec2 normalized() const { double len = length(); return Vec2(x / len, y / len); }

};