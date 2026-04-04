#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Color.h"

// Simulation Constants
const double G = 100.0;

// Light and Sun
const glm::vec3 LIGHT_POS(0.0f, 500.0f, 0.0f);
const Vec3 SUN_POS(0.0f, 500.0f, 0.0f);
const double SUN_MASS = 1000.0;
const CLR SUN_COLOR(1.0f, 0.8f, 0.0f);

// Mercury
const double MERCURY_DIST = 80.0;
const double MERCURY_MASS = 0.1;
const CLR MERCURY_COLOR(0.6f, 0.6f, 0.6f);

// Venus
const double VENUS_DIST = 150.0;
const double VENUS_MASS = 0.8;
const CLR VENUS_COLOR(0.9f, 0.8f, 0.5f);

// Earth
const double EARTH_DIST = 250.0;
const double EARTH_MASS = 1.0;
const CLR EARTH_COLOR(0.2f, 0.5f, 1.0f);

// Mars
const double MARS_DIST = 380.0;
const double MARS_MASS = 0.2;
const CLR MARS_COLOR(0.9f, 0.3f, 0.2f);

// Jupiter
const double JUPITER_DIST = 650.0;
const double JUPITER_MASS = 10.0;
const CLR JUPITER_COLOR(0.8f, 0.6f, 0.4f);

// Saturn
const double SATURN_DIST = 950.0;
const double SATURN_MASS = 8.0;
const CLR SATURN_COLOR(0.9f, 0.8f, 0.6f);

// Uranus
const double URANUS_DIST = 1250.0;
const double URANUS_MASS = 3.0;
const CLR URANUS_COLOR(0.6f, 0.9f, 0.9f);

// Neptune
const double NEPTUNE_DIST = 1550.0;
const double NEPTUNE_MASS = 3.0;
const CLR NEPTUNE_COLOR(0.2f, 0.3f, 0.8f);

#endif // CONSTANTS_H