#include "physics/Physics.h"
#include "shapes/Shape.h"

void Physics::integrate(Shape& s, glm::vec3 f) {
    // if (glm::any(glm::isnan(s.position))) 
    //     s.position = glm::vec3(ofGetWidth()/2, ofGetHeight()/2, 0);
    // if (glm::any(glm::isnan(vel))) 
    //     vel = glm::vec3(0,0,0);
    
    if (glm::any(glm::isnan(vel))) 
        vel = glm::vec3(0, 0, 0);


    // float dt = 1.0f/ofGetFrameRate();
    float dt = ofGetLastFrameTime();

    force += f;
    
    s.position += this->vel * dt;
    this->accel = (1.0f/mass) * force;
    this->vel += this->accel * dt;
    this->vel *= damping;  // Adjust to taste: 0.95 = gentle, 0.80 = strong


    force = glm::vec3(0, 0 ,0);
        

    // cout << "Velocity: " << vel << " | Position: " << s.position << endl;
}