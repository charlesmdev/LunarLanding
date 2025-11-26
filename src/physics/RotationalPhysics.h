#pragma once
#include "Physics.h"
#include "ofMain.h"

class RotationalPhysics : public Physics {
public:
    float angularVel = 0.0f;
    float angularAccel = 0.0f;
    float torque = 0.0f;
    float rotationalDamping = 0.99f;

    void integrate(Shape& s, glm::vec3 force) override {
        // float dt = 1.0f / ofGetFrameRate();
         float dt = ofGetLastFrameTime();

        s.rotation += angularVel * dt;
        angularAccel = torque / mass;
        angularVel += angularAccel * dt;
        angularVel *= rotationalDamping;
        

        torque = 0.0f;
        // cout << "AngularVel: " << angularVel << " | Rotation: " << s.rotation << endl;
    }

    void addTorque(float t) {
        torque += t;
        // cout << "Adding torque! " << endl;
    }
};

