#ifndef VEC3_H
#define VEC3_H

#include <cmath>

class Vec3 {

public:
    double x;
    double y;
    double z;

    Vec3(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z);}
    Vec3 operator*(double scalar)     const { return Vec3(x * scalar, y * scalar, z * scalar);}

    double length()   const { return std::sqrt(x * x + y * y + z * z);}
    Vec3 normalized() const { double len = length(); return Vec3(x / len, y / len, z / len); }    

};

#endif // VEC3_H