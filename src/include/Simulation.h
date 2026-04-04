#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <glm/glm.hpp>
#include "Body.h"
#include "Constants.h"

class Simulation {

private:
    std::vector<Body> objects;

public:
    Simulation() {
        objects.reserve(9);
    }

    void addBody(Body&& body) {
        objects.push_back(std::move(body));
    }

    void update(float dt) {

        std::vector<glm::vec3> accelerations(objects.size(), glm::vec3(0.0f, 0.0f, 0.0f));

        // Calculate gravitational forces
        for (size_t i = 0; i < objects.size(); ++i) {
            for (size_t j = 0; j < objects.size(); ++j) {

                if (i == j) continue;

                glm::vec3 diff = objects[j].position - objects[i].position;
                double distance = glm::length(diff);

                if (distance < 5.0f) distance = 5.0f;

                double forceMag = (G * objects[i].mass * objects[j].mass) / (distance * distance);
                glm::vec3 forceDir = glm::normalize(diff);

                accelerations[i] += forceDir * (static_cast<float>(forceMag) / static_cast<float>(objects[i].mass));

            }
        }

        // Update velocities and positions
        for (size_t i = 0; i < objects.size(); ++i) {
            objects[i].accelerate(accelerations[i] * dt);
            objects[i].updatePosition(dt);
        }

    }

    void render(GLuint shaderProgram, const glm::vec3& sunPos) {
        for (size_t i = 0; i < objects.size(); ++i) {
            objects[i].drawTrail(shaderProgram);
            objects[i].draw(shaderProgram, i == 0, sunPos);
        }
    }

};

#endif // SIMULATION_H