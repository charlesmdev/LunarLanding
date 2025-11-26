#pragma once
#include "ofMain.h"

class Shape;

class Physics {
    public:
        // glm::vec3 pos; // Unknown if needed since position is already in base shape class
        glm::vec3 vel = glm::vec3(0, 0, 0);
        glm::vec3 accel = glm::vec3(0, 0, 0);
        glm::vec3 force = glm::vec3(0, 0, 0);

        float damping = 0.99f;
        float mass = 0.8f;

        Physics(){}
        virtual void integrate(Shape& s, glm::vec3 f);
        void addForce(glm::vec3 f) {
            force += f;
        }
};