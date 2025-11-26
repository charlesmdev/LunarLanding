#include "Shape.h"
#include "physics/Physics.h"
#include "physics/RotationalPhysics.h"
#include "emitters/ParticleEmitter.h"
#include "physics/Collision.h"

#pragma once
class Player : public Shape {
    public:
        int lives;
        ParticleEmitter gun;
        ParticleEmitter explosion;
        Physics playerPhysics;
        RotationalPhysics playerRotPhysics;
        Collision col;

        ofPolyline polyShape;

        std::vector<glm::vec3> playerVerts = {
        glm::vec3( 0, -20, 0),   
        glm::vec3(-12, 10, 0),   
        glm::vec3(-6, 5, 0),     
        glm::vec3( 0, 10, 0),    
        glm::vec3( 6, 5, 0),     
        glm::vec3( 12, 10, 0)    
        };

        Player(); 
        void draw() override;
        void move(glm::vec3 thrust) override;
        void rotate(float r) override; // Unsure if it has a rotational force?

        glm::vec3 getCenterPoint() override;
        float getHeight() override;

            
        void updateGun();
        void fire();
        void stopFire();
        void die();
        
        bool showBoundingCircle = false;
        bool exploding = false;
        float explosionStartTime = 0;  // Track when explosion started

        void updateExplosion();
};