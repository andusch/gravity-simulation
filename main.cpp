#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// headers for OpenGL and GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/include/Body.h"
#include "src/include/Display.h"
#include "src/include/Shader.h"
#include "src/include/Input.h"
#include "src/include/Constants.h"
#include "src/include/Color.h"

GLFWwindow* StartGLFW();

int main() {

    // Setup Window
    GLFWwindow* window = StartGLFW();
    if(!window) return -1; // safety check
    
    /* ---------- Load and compile shaders ---------- */
    std::string vSourceStr = readFile("./src/shaders/vertex_shader.glsl");
    std::string fSourceStr = readFile("./src/shaders/fragment_shader.glsl");
    const char* vSource = vSourceStr.c_str();
    const char* fSource = fSourceStr.c_str();

    if (vSourceStr.empty() || fSourceStr.empty()) {
        std::cerr << "Shader files are empty or not found!" << std::endl;
        return -1;
    }

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vSource, NULL);
    glCompileShader(vs);
    checkCompileErrors(vs, "VERTEX");

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fSource, NULL);
    glCompileShader(fs);
    checkCompileErrors(fs, "FRAGMENT");

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    // Check shader linking
    checkShaderLinking(shaderProgram);

    /* ---------------------------------------------- */


    // Circular Orbit Velocity
    double v_orbit = std::sqrt((G * SUN_MASS) / EARTH_DIST);

    // Create Bodies
    std::vector<Body> objects;
    objects.reserve(2);
    objects.emplace_back(Vec2(640.0, 360.0), Vec2(0.0, 0.0), SUN_MASS, 35.0f, SUN_COLOR);
    objects.emplace_back(Vec2(640.0 + EARTH_DIST, 360.0), Vec2(0.0, v_orbit), EARTH_MASS, 10.0f, EARTH_COLOR);

    float dt = 0.016f; // Simulation time step per frame

    while(!glfwWindowShouldClose(window)) {
        // Clear Screen
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f); // Dark space blue
        glClear(GL_COLOR_BUFFER_BIT);

        // Set Projection (matches your screen dims)
        setOrtho(shaderProgram, 0.0f, 1280.0f, 0.0f, 720.0f);

        /* ------------- Gravity Simulation ------------- */
        std::vector<Vec2> accelerations(objects.size(), Vec2(0.0f, 0.0f)); // initialize accelerations

        for (size_t i = 0; i < objects.size(); ++i) {
            for(size_t j = 0; j < objects.size(); ++j) {

                if(i == j) continue;

                Vec2 diff = objects[j].position - objects[i].position;
                double distance = diff.length();

                if (distance < 5.0f) distance = 5.0f;

                double forceMag = (G * objects[i].mass * objects[j].mass) / (distance * distance);
                Vec2 forceDir = diff.normalized();

                // Newton's second law: a = F / m
                accelerations[i] = accelerations[i] + (forceDir * (forceMag / objects[i].mass));

            }
        }
        /* ---------------------------------------------- */

        /* ---------- Integration and rendering ---------- */
        glUseProgram(shaderProgram);
        for (size_t i = 0; i < objects.size(); ++i) {

            objects[i].accelerate(accelerations[i] * dt);         // update velocity
            objects[i].updatePosition();                          // update position

            objects[i].drawTrail(shaderProgram);                  // draw trail

            objects[i].draw(shaderProgram, objects[i].position);  // draw body

        }
        /* ---------------------------------------------- */

        glfwSwapBuffers(window);             // swap buffers front and back
        glfwPollEvents();                    // poll events (inputs, )
    }

    /* ------------------- Cleanup ------------------- */
    glDeleteProgram(shaderProgram);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glfwTerminate();
    /* ---------------------------------------------- */

    return 0;
}