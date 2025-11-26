#pragma once
#include "ofMain.h"
#include "Collision.h"
#include "particles/ParticleSystem.h"
#include <vector>
#include <memory>
#include <functional>

using namespace std;

class CollisionManager {
public:
    void addCollider(Collision* collider);
    void removeCollider(Collision* collider);

    // For projectile collision ;-;
    void addParticleSystem(ParticleSystem* system); 
    
    void checkCollisions();
    
    // Callback function for when collision occurs
    function<void(Collision*, Collision*)> onCollision; 
    function<void(glm::vec3, Collision*)> onParticleCollision;

    
    void clear();
    
private:
    vector<Collision*> colliders;
    vector<ParticleSystem*> particleSystems;

    void checkColliderPairs();
    void checkParticleCollisions();
};