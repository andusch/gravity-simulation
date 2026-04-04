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
#include "src/include/Shader.h"
#include "src/include/Display.h"
#include "src/include/Constants.h"
#include "src/include/PathUtils.h"
#include "src/include/Simulation.h"


// camera variables
float yaw = -90.0f;
float pitch = 0.0f;
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

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    
}

GLFWwindow* StartGLFW();

int main() {

    // Setup Window
    GLFWwindow* window = StartGLFW();
    if(!window) return -1; // safety check

    /* ---------- Load and compile shaders ---------- */
    std::string vSourceStr = readFile(PathUtils::getShaderPath("vertex_shader.glsl"));
    std::string fSourceStr = readFile(PathUtils::getShaderPath("fragment_shader.glsl"));
    const char* vSource = vSourceStr.c_str();
    const char* fSource = fSourceStr.c_str();

    if (vSourceStr.empty() || fSourceStr.empty()) {
        std::cerr << "Shader files are empty or not found!" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

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

    glEnable(GL_DEPTH_TEST); // Enable depth testing for proper 3D rendering
    checkShaderLinking(shaderProgram);

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

        // simple keyboard input for camera movement
        float cameraSpeed = 1000.0f * deltaTime; // adjust accordingly
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += cameraSpeed * cameraFront;
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraFront;
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);


        /* ---------- Render scene ---------- */
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Dark space blue
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        updateFPS(window);    // Start FPS counter

        // SETUP VIEW AND PROJECTION MATRICES
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 10000.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);

        grid.draw(shaderProgram);
        sim.render(shaderProgram, SUN_POS);

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