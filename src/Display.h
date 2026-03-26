#include <iostream>

// headers for OpenGL and GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* StartGLFW() {

    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

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
