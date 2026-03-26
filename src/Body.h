#include "Vec2.h"
#include <GLFW/glfw3.h>

class Body {

public:

    Vec2 position;
    Vec2 velocity;
    double radius;
    double mass;

    Body(Vec2 position, Vec2 velocity, double mass, double radius = 15.0) {
        this -> position = position;
        this -> velocity = velocity;
        this -> radius = radius;
        this -> mass = mass;
    }

    void accelerate(const Vec2& acceleration) {
        velocity = velocity + acceleration;
    }

    void updatePosition(){
        position = position + (velocity * (1.0f / 1000.0f));
    }

    void drawCircle(){
        drawCircle(position, radius, 100);
    }

    void drawCircle(const Vec2& center, float radius, int resolution) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(position.x, position.y); // center of the circle
        for(int i = 0; i <= resolution; ++i) {
            float angle = i * 2.0f * M_PI / resolution;
            float x = position.x + radius * cos(angle);
            float y = position.y + radius * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();
    }

};