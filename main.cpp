#include <iostream>

#include <vector>
#include <math.h>

// headers for OpenGL and GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./src/Display.h"

// header for the Body class
#include "./src/Body.h"

// screen dimensions
const float screenWidth = 1280.0f;
const float screenHeight = 720.0f;

// physics constants
const float G = 6.67430 * pow(10, -11); // gravitational constant

GLFWwindow* StartGLFW();

int main() {

    GLFWwindow* window = StartGLFW();
    if(!window) return -1;             // safety check

    glMatrixMode(GL_PROJECTION);                     // set up projection matrix
    glLoadIdentity();                                // reset projection matrix
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1); // set up orthographic projection
    glMatrixMode(GL_MODELVIEW);                      // switch to modelview matrix
    glLoadIdentity();                                // reset modelview matrix

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;
    float radius = 15.0f;
    int resolution = 100;

    std::vector<Body> objects = {
        Body(Vec2(200.f, 300.f), Vec2(0.0f, 1500.0f), 7.35 * pow(10, 22)),
        Body(Vec2(700.f, 300.f), Vec2(0.0f, -1500.0f), 7.35 * pow(10, 22)),
    };

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);         // clear the color buffer

        for(auto& obj : objects) {

            // calculate gravitational forces from other objects
            for(auto& other : objects) {
                
                if(&obj == &other) continue; // skip self

                float dx = other.position.x - obj.position.x;
                float dy = other.position.y - obj.position.y;
                float distance = sqrt(dx * dx + dy * dy);
                Vec2 direction = Vec2(dx / distance, dy / distance);
                distance *= 1000; // scale distance for better simulation

                float GForce = (G * obj.mass * other.mass) / (distance * distance);
                float acc1 = GForce / obj.mass;

                Vec2 accVector = {direction.x * acc1, direction.y * acc1};
                obj.accelerate(accVector);

            }
            
            obj.updatePosition();
            obj.drawCircle();

            if(obj.position.y < 0 || obj.position.y > screenHeight) {
                obj.velocity.y *= -0.95f;
            }
            if(obj.position.x < 0 || obj.position.x > screenWidth) {
                obj.velocity.x *= -0.95f;
            }
            
        }

        glfwSwapBuffers(window);             // swap buffers front and back
        glfwPollEvents();                    // poll events (inputs, )
    }

    glfwTerminate();
    return 0;
}