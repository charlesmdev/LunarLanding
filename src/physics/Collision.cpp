#include "Collision.h"
#include "shapes/Shape.h"

using namespace std;

Collision::Collision(Shape* owner, float radius) : owner(owner), radius(radius) {}

bool Collision::checkCollision(Collision* other) {
    if (!owner || !other || !other->getOwner()) return false;
    float distance = glm::distance(owner->getPosition(), other->getOwner()->getPosition());
    return distance < (radius + other->getRadius());
}

bool Collision::checkPointCollision(glm::vec3 point) {
    if (!owner) return false;
    float distance = glm::distance(owner->getPosition(), point);
    return distance < radius;
}