#ifndef BODY_H
#define BODY_H

#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <deque>

#include "Vec2.h"

class Body {

public:

    Vec2 position;
    Vec2 velocity;
    double mass;
    float radius;

    GLuint vao, vbo;
    int vertexCount;

    std::deque<Vec2> history;
    const size_t maxHistory = 400;

    Body(Vec2 position, Vec2 velocity, double mass, float radius = 15.0f) : position(position), velocity(velocity), mass(mass), radius(radius) {
        setupMesh();
    }

    void accelerate(const Vec2& acceleration) {
        velocity = velocity + acceleration;

        // Store history
        history.push_back(position);
        if (history.size() > maxHistory) {
            history.pop_front();
        }
    }

    void updatePosition(){
        position = position + (velocity * 0.016f); 
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

    void drawTrail(GLuint shaderProgram) {
        if (history.size() < 2) return;

        std::vector<float> trailVertices;
        for (const Vec2& pos : history) {
            trailVertices.push_back((float)pos.x);
            trailVertices.push_back((float)pos.y);
        }

        GLuint tVAO, tVBO;
        glGenVertexArrays(1, &tVAO);
        glGenBuffers(1, &tVBO);

        glBindVertexArray(tVAO);

        glBindBuffer(GL_ARRAY_BUFFER, tVBO);
        glBufferData(GL_ARRAY_BUFFER, trailVertices.size() * sizeof(float), trailVertices.data(), GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glUniform2f(glGetUniformLocation(shaderProgram, "offset"), 0.0f, 0.0f);

        glDrawArrays(GL_LINE_STRIP, 0, history.size());

        glBindVertexArray(0); 
        glDeleteBuffers(1, &tVBO);
        glDeleteVertexArrays(1, &tVAO);

    }

};

#endif // BODY_H