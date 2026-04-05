#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <glm/glm.hpp>
#include "Body.h"
#include "Constants.h"

struct State {
    glm::vec3 position;
    glm::vec3 velocity;
};

struct Derivative {
    glm::vec3 dPosition;
    glm::vec3 dVelocity;
};

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

        for (size_t i = 0; i < objects.size(); ++i) {

            State state = { objects[i].position, objects[i].velocity };

            Derivative a = evaluate(state, 0.0f, {glm::vec3(0.0f), glm::vec3(0.0f)}, i);
            Derivative b = evaluate(state, dt * 0.5f, a, i);
            Derivative c = evaluate(state, dt * 0.5f, b, i);
            Derivative d = evaluate(state, dt, c, i);

            glm::vec3 dPositiondt = 1.0f / 6.0f * (a.dPosition + 2.0f * (b.dPosition + c.dPosition) + d.dPosition);
            glm::vec3 dVelocitydt = 1.0f / 6.0f * (a.dVelocity + 2.0f * (b.dVelocity + c.dVelocity) + d.dVelocity);

            objects[i].position += dPositiondt * dt;
            objects[i].velocity += dVelocitydt * dt;

            objects[i].history.push_back(objects[i].position);
            if (objects[i].history.size() > objects[i].maxHistory) {
                objects[i].history.pop_front();
            }

        }

    }

    void render(GLuint shaderProgram, const glm::vec3& sunPos) {
        for (size_t i = 0; i < objects.size(); ++i) {
            objects[i].drawTrail(shaderProgram);
            objects[i].draw(shaderProgram, i == 0, sunPos);
        }
    }

private:
    glm::vec3 calculateAcceleration(const glm::vec3& pos, size_t index) {
        
        glm::vec3 acceleration(0.0f);

        for (size_t j = 0; j < objects.size(); ++j) {
            
            if (index == j) continue;
            glm::vec3 diff = objects[j].position - pos;
            float distSq = glm::dot(diff, diff);
            float distance = std::sqrt(distSq);

            if (distance < 5.0f) distance = 5.0f;

            float forceMag = (G * objects[j].mass) / (distSq); 
            acceleration += glm::normalize(diff) * (float)forceMag;
        }

        return acceleration;
    }

    Derivative evaluate(const State& initial, float dt, const Derivative& d, size_t index) {

        State state;
        state.position = initial.position + d.dPosition * dt;
        state.velocity = initial.velocity + d.dVelocity * dt;

        Derivative output;
        output.dPosition = state.velocity;
        output.dVelocity = calculateAcceleration(state.position, index);
        return output;

    }

};

#endif // SIMULATION_H