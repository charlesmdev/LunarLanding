#pragma once
#include "ofMain.h"

using namespace std;

class Shape;

class Collision {
public:
    Collision(Shape* owner, float radius);
    
    bool checkCollision(Collision* other);
    bool checkPointCollision(glm::vec3 point);
    
    float getRadius() const { return radius; }
    void setRadius(float r) { radius = r; }
    
    Shape* getOwner() { return owner; }
    
private:
    Shape* owner;
    float radius;
};