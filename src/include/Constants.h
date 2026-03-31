#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Color.h"

// Simulation Constants
const double G = 100.0;

const glm::vec3 LIGHT_POS(0.0f, 0.0f, 0.0f);
const double SUN_MASS = 1000.0;
const CLR SUN_COLOR(1.0f, 0.8f, 0.0f);

const double EARTH_MASS = 1.0;
const double EARTH_DIST = 250.0;
const CLR EARTH_COLOR(0.4f, 0.6f, 1.0f);

#endif // CONSTANTS_H