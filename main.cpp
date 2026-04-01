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
    std::string vSourceStr = readFile("./src/shaders/vertex_shader.glsl");
    std::string fSourceStr = readFile("./src/shaders/fragment_shader.glsl");
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

    // Check shader linking
    checkShaderLinking(shaderProgram);

    /* ---------------------------------------------- */


    // Circular Orbit Velocity
    double v_orbit = std::sqrt((G * SUN_MASS) / EARTH_DIST);

    // Create Bodies
    std::vector<Body> objects;
    objects.reserve(5);
    objects.emplace_back(Vec3(0.0, 100.0, 0.0), Vec3(0.0, 0.0, 0.0), SUN_MASS, 35.0f, SUN_COLOR);
    objects.emplace_back(Vec3(EARTH_DIST, 100.0, 0.0), Vec3(0.0, 0.0, v_orbit), EARTH_MASS, 10.0f, EARTH_COLOR); // EARTH
    objects.emplace_back(Vec3(-EARTH_DIST * 1.5, 100.0, 0.0), Vec3(0.0, 0.0, -v_orbit * 0.8), EARTH_MASS * 0.5, 7.0f, CLR(1.0f, 0.4f, 0.4f)); // RED DWARF
    objects.emplace_back(Vec3(EARTH_DIST * 1.5, 100.0, 0.0), Vec3(0.0, 0.0, v_orbit * 0.8), EARTH_MASS * 0.5, 7.0f, CLR(1.0f, 0.4f, 0.4f)); // RED DWARF
    objects.emplace_back(Vec3(0.0, 100.0, -EARTH_DIST * 1.5), Vec3(v_orbit * 0.8, 0.0, 0.0f), EARTH_MASS * 0.5, 7.0f, CLR(1.0f, 0.4f, 0.4f)); // RED DWARF


    float dt = 0.016f; // Simulation time step per frame

    SpaceGrid grid(5000.0f, 50); // Large grid for better visibility

    while(!glfwWindowShouldClose(window)) {
        // Clear Screen
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Dark space blue
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // SETUP VIEW AND PROJECTION MATRICES
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 10000.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Calculate delta time for consistent movement speed
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // simple keyboard input for camera movement
        float cameraSpeed = 500.0f * deltaTime; // adjust accordingly
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += cameraSpeed * cameraFront;
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraFront;
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));


        /* ------------- Gravity Simulation ------------- */
        std::vector<Vec3> accelerations(objects.size(), Vec3(0.0f, 0.0f, 0.0f)); // initialize accelerations

        for (size_t i = 0; i < objects.size(); ++i) {
            for(size_t j = 0; j < objects.size(); ++j) {

                if(i == j) continue;

                Vec3 diff = objects[j].position - objects[i].position;
                double distance = diff.length();

                if (distance < 5.0f) distance = 5.0f;

                double forceMag = (G * objects[i].mass * objects[j].mass) / (distance * distance);
                Vec3 forceDir = diff.normalized();

                // Newton's second law: a = F / m
                accelerations[i] = accelerations[i] + (forceDir * (forceMag / objects[i].mass));

            }
        }
        /* ---------------------------------------------- */

        /* ---------- Integration and rendering ---------- */
        glUseProgram(shaderProgram);

        grid.draw(shaderProgram); // Draw the space grid

        for (size_t i = 0; i < objects.size(); ++i) {

            objects[i].accelerate(accelerations[i] * dt);         // update velocity
            objects[i].updatePosition();                          // update position

            objects[i].drawTrail(shaderProgram);                  // draw trail

            objects[i].draw(shaderProgram, i == 0, LIGHT_POS);    // draw body

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