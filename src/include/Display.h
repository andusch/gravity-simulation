#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>

// headers for OpenGL and GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* StartGLFW() {

    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS
#endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Gravity Simulation: Solar System", NULL, NULL);

    if(!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;

}

void updateFPS(GLFWwindow* window) {
    
    static double previousTime = glfwGetTime();
    static int frameCount = 0;
    double currentTime = glfwGetTime();
    double elapsedTime = currentTime - previousTime;
    
    frameCount++;

    if (elapsedTime >= 1.0) {
        double fps = frameCount / elapsedTime;
        std::string title = "Gravity Simulation: Solar System - FPS: " + std::to_string(static_cast<int>(fps));
        glfwSetWindowTitle(window, title.c_str());
        previousTime = currentTime;
        frameCount = 0;
    }

}

#endif // DISPLAY_H