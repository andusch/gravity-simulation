#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>

class Body {

public:

    Vec2 position;
    Vec2 velocity;
    double mass;
    float radius;

    GLuint vao, vbo;
    int vertexCount;

    Body(Vec2 position, Vec2 velocity, double mass, float radius = 15.0f) : position(position), velocity(velocity), mass(mass), radius(radius) {
        setupMesh();
    }

    void setupMesh() {
        
        int resolution = 50;
        
        std::vector<float> vertices;
        vertices.push_back(0.0f); // center x
        vertices.push_back(0.0f); // center y

        for (int i = 0; i <= resolution; ++i) {
            float angle = i * 2.0f * M_PI / resolution;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            vertices.push_back(x);
            vertices.push_back(y);
        }

        vertexCount = vertices.size() / 2;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
    }

    void draw(GLuint shaderProgram, const Vec2& screenCenter) {
        glUseProgram(shaderProgram);
        glUniform2f(glGetUniformLocation(shaderProgram, "offset"), (float)position.x, (float)position.y);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
        glBindVertexArray(0);
    }

    void accelerate(const Vec2& acceleration) {
        velocity = velocity + acceleration;
    }

    void updatePosition(){
        position = position + (velocity * 0.016f); 
    }

};

#endif // BODY_H