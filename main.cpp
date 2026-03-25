#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <math.h>

GLFWwindow* StartGLFW();

const float screenWidth = 1280.0f;
const float screenHeight = 720.0f;

const float G = 6.67430 * pow(10, -11); // gravitational constant

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

class Object {

    public:
    
    std::vector<float> position;
    std::vector<float> velocity;
    float radius;
    float mass;

    Object(std::vector<float> position, std::vector<float> velocity, float mass, float radius = 15.0f) {
        this -> position = position;
        this -> velocity = velocity;
        this -> radius = radius;
        this -> mass = mass;
    }

    void accelerate(float x, float y) {
        velocity[0] += x;
        velocity[1] += y;
    }

    void updatePosition(){
        this -> position[0] += this -> velocity[0] / 1000.0f;
        this -> position[1] += this -> velocity[1] / 1000.0f;
    }

    void drawCircle(){
        drawCircle(position[0], position[1], radius, 100);
    }

    void drawCircle(float centerX, float centerY, float radius, int resolution) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(centerX, centerY); // center of the circle
        for(int i = 0; i <= resolution; ++i) {
            float angle = i * 2.0f * M_PI / resolution;
            float x = centerX + radius * cos(angle);
            float y = centerY + radius * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();
    }

};

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

    std::vector<Object> objects = {
        Object(std::vector<float>{200.f, 300.f}, std::vector<float>{0.0f, 1500.0f}, 7.35 * pow(10, 22)),
        Object(std::vector<float>{700.f, 300.f}, std::vector<float>{0.0f, -1500.0f}, 7.35 * pow(10, 22)),
    };

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);         // clear the color buffer

        for(auto& obj : objects) {

            for(auto& other : objects) {
                
                if(&obj == &other) continue; // skip self

                float dx = other.position[0] - obj.position[0];
                float dy = other.position[1] - obj.position[1];
                float distance = sqrt(dx * dx + dy * dy);
                std::vector<float> direction = {dx / distance, dy / distance};
                distance *= 1000; // scale distance for better simulation

                float GForce = (G * obj.mass * other.mass) / (distance * distance);
                float acc1 = GForce / obj.mass;

                std::vector<float> accVector = {direction[0] * acc1, direction[1] * acc1};
                obj.accelerate(accVector[0], accVector[1]);

            }
            
            obj.updatePosition();
            obj.drawCircle();

            if(obj.position[1] < 0 || obj.position[1] > screenHeight) {
                obj.velocity[1] *= -0.95f;
            }
            if(obj.position[0] < 0 || obj.position[0] > screenWidth) {
                obj.velocity[0] *= -0.95f;
            }
            
        }

        glfwSwapBuffers(window);             // swap buffers front and back
        glfwPollEvents();                    // poll events (inputs, )
    }

    glfwTerminate();
    return 0;
}