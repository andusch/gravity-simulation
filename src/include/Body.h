#ifndef BODY_H
#define BODY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <deque>

#include "Vec3.h"
#include "Color.h"

class Body {

public:

    Vec3 position;
    Vec3 velocity;
    double mass;
    float radius;
    // Vec3 color;
    CLR clr;

    GLuint vao, vbo, ebo;
    int indexCount;

    std::deque<Vec3> history;
    const size_t maxHistory = 5000;

    Body(Vec3 position, Vec3 velocity, double mass, float radius = 15.0f, CLR clr = CLR(1.0f, 1.0f, 1.0f)) : position(position), velocity(velocity), mass(mass), radius(radius), clr(clr) {
        setupMesh();
    }

    void accelerate(const Vec3& acceleration) {
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
        
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        int sectorCount = 36; // Longitude lines
        int stackCount = 18;  // Latitude lines
        float sectorStep = 2 * M_PI / sectorCount;
        float stackStep = M_PI / stackCount;

        // 1. Generate 3D vertices for a sphere
        for (int i = 0; i <= stackCount; ++i) {

            float stackAngle = M_PI / 2 - i * stackStep;        
            float xy = radius * cosf(stackAngle);             
            float z = radius * sinf(stackAngle);

            for(int j = 0; j <= sectorCount; ++j) {
                float sectorAngle = j * sectorStep;           
                float x = xy * cosf(sectorAngle);             
                float y = xy * sinf(sectorAngle);             
                vertices.push_back(x); vertices.push_back(y); vertices.push_back(z);
                vertices.push_back(x / radius); vertices.push_back(y / radius); vertices.push_back(z / radius); // Normalized for lighting
            }

        }

        // 2. Generate indices for the sphere's triangles

        for (int i = 0; i < stackCount; ++i) {
            
            int k1 = i * (sectorCount + 1);
            int k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
                if (i != (stackCount - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }

        }

        indexCount = indices.size();

        // 3. Setup OpenGL buffers
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo); // New EBO for index drawing

        glBindVertexArray(vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        
    }

    void draw(GLuint shaderProgram, bool sunMode, glm::vec3 currentLightPos) {
        glUseProgram(shaderProgram);

        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), clr.r, clr.g, clr.b);

        glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), currentLightPos.x, currentLightPos.y, currentLightPos.z);

        glUniform1i(glGetUniformLocation(shaderProgram, "isEmitter"), sunMode);


        // CREATE MODEL MATRIX: Move the sphere to its actual 3D position
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(vao);
        // Draw using Elements (Triangles) instead of Arrays
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void drawTrail(GLuint shaderProgram) {
        if (history.size() < 2) return;

        std::vector<float> trailVertices;
        for (const Vec3& pos : history) {
            trailVertices.push_back((float)pos.x);
            trailVertices.push_back((float)pos.y);
            trailVertices.push_back((float)pos.z); // Added Z axis
        }

        GLuint tVAO, tVBO;
        glGenVertexArrays(1, &tVAO);
        glGenBuffers(1, &tVBO);

        glBindVertexArray(tVAO);

        glBindBuffer(GL_ARRAY_BUFFER, tVBO);
        glBufferData(GL_ARRAY_BUFFER, trailVertices.size() * sizeof(float), trailVertices.data(), GL_DYNAMIC_DRAW);

        // Attribute pointer now reads 3 floats (X, Y, Z)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glUniform1f(glGetUniformLocation(shaderProgram, "isEmitter"), true);
        // glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), clr.r * 0.8f, clr.g * 0.8f, clr.b * 1.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 1.0f, 1.0f); // Trail is always white for better visibility
        
        
        // For the trail, vertices are ALREADY in world space, so we use an Identity Matrix (No translation)
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_LINE_STRIP, 0, history.size());

        glBindVertexArray(0); 
        glDeleteBuffers(1, &tVBO);
        glDeleteVertexArrays(1, &tVAO);

    }

};

#endif // BODY_H