#include "CollisionManager.h"

using namespace std;

void CollisionManager::addCollider(Collision* collider) {
    colliders.push_back(collider);
}

void CollisionManager::addParticleSystem(ParticleSystem* system) {
    particleSystems.push_back(system);
}

void CollisionManager::removeCollider(Collision* collider) {
    colliders.erase(
        remove(colliders.begin(), colliders.end(), collider),
        colliders.end()
    );
}

void CollisionManager::checkCollisions() {
    checkColliderPairs();
    checkParticleCollisions();
}

void CollisionManager::checkColliderPairs() {
    // Check all collider pairs
    for (size_t i = 0; i < colliders.size(); i++) {
        for (size_t j = i + 1; j < colliders.size(); j++) {
            if (colliders[i]->checkCollision(colliders[j])) { // Hey these are colliding
                if (onCollision) { // Statement to check if there is already an onCollision call going ong
                    onCollision(colliders[i], colliders[j]);
                }
            }
        }
    }
}
void CollisionManager::checkParticleCollisions() {
    // Check particles against all colliders
    for (auto* system : particleSystems) {
        for (auto& particle : system->particles) {
            glm::vec3 particlePos(particle.position.x, particle.position.y, particle.position.z);
            
            for (auto* collider : colliders) {
                if (collider->checkPointCollision(particlePos)) {
                    // Trigger callback if set
                    if (onParticleCollision) {
                        onParticleCollision(particlePos, collider);
                    }
                    particle.lifespan = 0;  // Kill particle
                }
            }
        }
    }
}
void CollisionManager::clear() {
    colliders.clear();
}