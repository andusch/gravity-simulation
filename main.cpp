#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// headers for OpenGL and GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// headers for GLM (math library)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/include/Grid.h"
#include "src/include/Body.h"
#include "src/include/Color.h"
#include "src/include/Input.h"
#include "src/include/Camera.h"
#include "src/include/Display.h"
#include "src/include/Constants.h"
#include "src/include/PathUtils.h"
#include "src/include/Simulation.h"
#include "src/include/ShaderProgram.h"

Camera camera(glm::vec3(0.0f, 500.0f, 600.0f));
float lastX = 1280.0f / 2.0f;
float lastY = 720.0f / 2.0f;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
 
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
    
}

GLFWwindow* StartGLFW();

int main() {

    // Setup Window
    GLFWwindow* window = StartGLFW();
    if(!window) return -1; // safety check

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    glEnable(GL_DEPTH_TEST); // Enable depth testing for proper 3D rendering

    ShaderProgram shader("vertex_shader.glsl", "fragment_shader.glsl");

    /* ---------------------------------------------- */


    /* ---------- Create celestial bodies ---------- */
    Simulation sim;
    sim.addBody(Body(SUN_POS, glm::vec3(0.0f, 0.0f, 5.0f), SUN_MASS, 35.0f, SUN_COLOR));

    auto addPlanet = [&](double distance, double mass, float radius, CLR color) {
        double v = std::sqrt(G * SUN_MASS / distance);
        sim.addBody(Body(glm::vec3(distance, SUN_POS.y, 0.0f), glm::vec3(0.0f, v, 0.0f), mass, radius, color));
    };

    addPlanet(MERCURY_DIST, MERCURY_MASS, 10.0f, MERCURY_COLOR);
    addPlanet(VENUS_DIST, VENUS_MASS, 12.0f, VENUS_COLOR);
    addPlanet(EARTH_DIST, EARTH_MASS, 14.0f, EARTH_COLOR);
    addPlanet(MARS_DIST, MARS_MASS, 11.0f, MARS_COLOR);
    addPlanet(JUPITER_DIST, JUPITER_MASS, 25.0f, JUPITER_COLOR);
    addPlanet(SATURN_DIST, SATURN_MASS, 22.0f, SATURN_COLOR);
    addPlanet(URANUS_DIST, URANUS_MASS, 18.0f, URANUS_COLOR);
    addPlanet(NEPTUNE_DIST, NEPTUNE_MASS, 17.0f, NEPTUNE_COLOR);

    SpaceGrid grid(5000.0f, 50); // Large grid for better visibility

    const float FIXED_DT = 0.016f; // Fixed time step for physics updates (60 FPS)
    float accumulator = 0.0f;
    lastFrame = glfwGetTime();

    while(!glfwWindowShouldClose(window)) {

        // Time management for fixed physics updates
        float currentFrame = glfwGetTime();
        float frameTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Prevent "spiral of death" in case of very long frame times
        if (frameTime > 0.25f) frameTime = 0.25f;

        deltaTime = frameTime;
        accumulator += frameTime;

        while (accumulator >= FIXED_DT) {
            sim.update(FIXED_DT);
            accumulator -= FIXED_DT;
        }

        // camera controls
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
        


        /* ---------- Render scene ---------- */
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Dark space blue
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        updateFPS(window);    // Start FPS counter

        // SETUP VIEW AND PROJECTION MATRICES
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 10000.0f);
        shader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        shader.setVec3("viewPos", camera.Position);

        grid.draw(shader.ID);
        sim.render(shader.ID, SUN_POS);

        glfwSwapBuffers(window);             // swap buffers front and back
        glfwPollEvents();                    // poll events (inputs, )
    }

    /* ------------------- Cleanup ------------------- */
    glfwTerminate();
    /* ---------------------------------------------- */

    return 0;
}