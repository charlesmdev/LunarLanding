#pragma once
#include "Shape.h"
#include "physics/Physics.h"
#include "physics/RotationalPhysics.h"
#include "utils/Helpers.h"
#include "physics/Collision.h"
#include "emitters/ParticleEmitter.h"
#include <vector>

class Asteroid : public Shape {
public:
    Physics linearPhysics;
    RotationalPhysics rotationalPhysics;
    Collision col;
    ParticleEmitter explosion;
    float baseRadius;
    int numSplits = 0;
    int generation = 0; // Used to count how many parent asteroids for spawning in the game. Too many parents is bad.

    Asteroid(int splitCount);
    Asteroid(glm::vec3 pos, float radius, int splitCount);

    void draw() override;
    void move(glm::vec3 force) override;
    void rotate(float torque) override;

    bool showBoundingCircle = false;

    bool exploding = false;
    float explosionStartTime = 0;

    void die();
    void updateExplosion();

    bool visible = true;

    // Coding splitting of asteroids :D
    vector<Asteroid*> split();
    // vector<Asteroid*> split(int numSplits = 3);
    float getRadius() const { return col.getRadius(); } // Since collision already has the radius of the object from randomGenerate func.

private:
    void generateRandomShape(float radius);
    void initializeVelocity();
    void initializeExplosion();
};