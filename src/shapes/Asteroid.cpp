#include "Asteroid.h"

Asteroid::Asteroid(int splitCount) : col(this, 30.0f) {
    // Spawn randomly on screen edge
    position = Helpers::randomSpawn();

    initializeVelocity();
    initializeExplosion();

    this->numSplits = splitCount;
    baseRadius = ofRandom(50, 70);
    generateRandomShape(baseRadius);
}

// This method is for creating a child asteroid at parent asteroid's current position
Asteroid::Asteroid(glm::vec3 pos, float radius, int splitCount) : col(this, radius) {
    position = pos;
    baseRadius = radius;
    
    initializeVelocity();
    initializeExplosion();
    
    this->numSplits = 0;
    generateRandomShape(radius);
}

void Asteroid::initializeVelocity() {
    // Asteroid has no thrust, so we just have a random initial velocity for each asteroid.
    float speed = ofRandom(100, 500); 
    float angle = ofRandom(0, TWO_PI);
    linearPhysics.vel = glm::vec3(cos(angle) * speed, sin(angle) * speed, 0);
    rotationalPhysics.angularVel = ofRandom(-90, 90); 
}
void Asteroid::initializeExplosion() {
    explosion.setEmitterType(RadialEmitter);
    explosion.setRate(50);
    explosion.setParticleRadius(2);
    explosion.setLifespan(1.0f);
    explosion.visible = true;
    explosion.started = false;
}

void Asteroid::generateRandomShape(float radius) {
    vertices.clear();
    int numVertices = 10;
    // float baseRadius = ofRandom(20, 60);
    col.setRadius(radius);

    float angleStep = TWO_PI / numVertices;
    for (int i = 0; i < numVertices; ++i) {
        float angle = i * angleStep + ofRandom(-angleStep * 0.3f, angleStep * 0.3f);
        float vertexRadius = radius * ofRandom(0.7f, 1.0f);
        vertices.push_back(glm::vec3(cos(angle) * vertexRadius, sin(angle) * vertexRadius, 0));
    }
}

vector<Asteroid*> Asteroid::split() {
    vector<Asteroid*> children;

    // minimum radius threshold
    if (baseRadius < 20.0f) {
        return children;
    }
    float newRadius = baseRadius * ofRandom(0.5f, 0.6f);
    
    for (int i = 0; i < numSplits; i++) {
        glm::vec3 offset(ofRandom(-20, 20), ofRandom(-20, 20), 0);
        glm::vec3 childPos = position + offset;
        
        Asteroid* child = new Asteroid(childPos, newRadius, this->numSplits);
        child->generation = this->generation + 1;  // COunts number of parent asteroids.

        
        // Adds to velocity of child.
        float speed = ofRandom(200, 600);
        float angle = ofRandom(0, TWO_PI);
        child->linearPhysics.vel = glm::vec3(cos(angle) * speed, sin(angle) * speed, 0);
        
        children.push_back(child);
    }
    
    return children;
}

void Asteroid::draw() {
    if (!exploding && visible) {
        ofPushMatrix();
        ofTranslate(position);
        ofRotateDeg(rotation);
        ofSetColor(200);

        ofBeginShape();
        for (auto& v : vertices) {
            ofVertex(v.x, v.y);
        }
        ofEndShape(true);
        ofPopMatrix();

        if(showBoundingCircle) ofDrawCircle(position.x, position.y, col.getRadius());
    }
    
    if (exploding) {
        explosion.draw();
    }
}

void Asteroid::move(glm::vec3 force) {
    // float turbulenceStrength = 1000.0f; // tweak for more or less jitter
    // glm::vec3 turbulentForce(ofRandom(-turbulenceStrength, turbulenceStrength), ofRandom(-turbulenceStrength, turbulenceStrength), 0.0f);
    // linearPhysics.addForce(turbulentForce); There's no turbulence in space!!! >:(
    linearPhysics.damping = 1.0f;
    linearPhysics.integrate(*this, force);
    Helpers::wrapAround(position);
}

void Asteroid::rotate(float torque) {
    // rotationalPhysics.torque = torque;
    // rotationalPhysics.torque += torque;
    rotationalPhysics.damping = 0.85f;
    rotationalPhysics.addTorque(torque);
    rotationalPhysics.integrate(*this, glm::vec3(0,0,0));

    explosion.sys->update();
    updateExplosion();
}

void Asteroid::die() {
    // cout << "Asteroid exploding!" << endl;
    
    exploding = true;
    explosionStartTime = ofGetElapsedTimeMillis();
    
    explosion.position = ofVec3f(position.x, position.y, position.z);
    explosion.start();
    
    col.setRadius(0); // Disable collision
}

void Asteroid::updateExplosion() {
    
    if (!exploding) return;
    // visible = false;

    explosion.update();
    if (ofGetElapsedTimeMillis() - explosionStartTime > 1500) {
        exploding = false;
        visible = false; // Wait until explosion is finished before asteroid manager deletes this obj from memory.
    }
}