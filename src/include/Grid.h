#ifndef GRID_H
#define GRID_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SpaceGrid {

public:
    GLuint vao, vbo;
    int vertexCount;

    SpaceGrid(float size, int divisions) {
        
        std::vector<float> vertices;
        float step = size / divisions;
        float halfSize = size / 2.0f;

        for (int i = 0; i <= divisions; ++i) {

            float pos = -halfSize + i * step;
            // Vertical line
            vertices.push_back(pos); vertices.push_back(0.0f); vertices.push_back(-halfSize);
            vertices.push_back(pos); vertices.push_back(0.0f); vertices.push_back(halfSize);

            // Horizontal line
            vertices.push_back(-halfSize); vertices.push_back(0.0f); vertices.push_back(pos);
            vertices.push_back(halfSize); vertices.push_back(0.0f); vertices.push_back(pos);

        }

        vertexCount = vertices.size() / 3;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

    }

    void draw(GLuint shaderProgram) {

        glUseProgram(shaderProgram);

        glUniform1i(glGetUniformLocation(shaderProgram, "isEmitter"), true);
        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.2f, 0.2f, 0.3f);

        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(vao);
        glDrawArrays(GL_LINES, 0, vertexCount);
        glBindVertexArray(0);

    }

};

#endif // GRID_H