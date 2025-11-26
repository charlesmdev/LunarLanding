//  Kevin M. Smith - CS 134 SJSU

#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() {
    sys = new ParticleSystem();
    createdSys = true;
    init();
}

ParticleEmitter::ParticleEmitter(ParticleSystem *s) {
    if (s == NULL)
    {
        cout << "fatal error: null particle system passed to ParticleEmitter()" << endl;
        ofExit();
    }
    sys = s;
    createdSys = false;
    init();
}

ParticleEmitter::~ParticleEmitter() {

    // deallocate particle system if emitter created one internally
    //
    if (createdSys) delete sys;
}

void ParticleEmitter::init() {
    rate = 1;
    velocity = ofVec3f(0, 20, 0);
    lifespan = 3;
    started = false;
    lastSpawned = 0;
    radius = 1;
    particleRadius = .1;
    visible = true;
    type = DirectionalEmitter;
}



void ParticleEmitter::draw() {
    if (visible) {
        switch (type) {
        case DirectionalEmitter:
            ofDrawSphere(position, radius/10);  // just draw a small sphere as a placeholder
            break;
        case SphereEmitter:
        case RadialEmitter:
            ofDrawSphere(position, radius/10);  // just draw a small sphere as a placeholder
            break;
        default:
            break;
        }
    }
    sys->draw();  
}
void ParticleEmitter::start() {
    started = true;
    lastSpawned = ofGetElapsedTimeMillis();
}

void ParticleEmitter::stop() {
    started = false;
}
void ParticleEmitter::update() {
    if (!started) return;

    float time = ofGetElapsedTimeMillis();

    if ((time - lastSpawned) > (1000.0 / rate)) {

        // set initial velocity and position based on emitter type
        switch (type) {
        case RadialEmitter: {
            // Check if this is the first spawn (lastSpawned will be very close to current time on first call)
            // Spawn burst immediately and stop
            for (int i = 0; i < 30; i++) {
                Particle particle;
                float angle = ofRandom(0, TWO_PI);
                float speed = ofRandom(100, 500);
                particle.velocity = ofVec3f(cos(angle) * speed, sin(angle) * speed, 0);
                particle.position.set(position);
                
                particle.lifespan = lifespan;
                particle.birthtime = time;
                particle.radius = particleRadius;
                
                sys->add(particle);
            }
            started = false;  // Stop after one burst for explosion effect
            break;
        }
        case SphereEmitter: {
            break;
        }
        case DirectionalEmitter: {
            Particle particle;
            particle.velocity = velocity;
            particle.position.set(position);
            
            particle.lifespan = lifespan;
            particle.birthtime = time;
            particle.radius = particleRadius;
            
            sys->add(particle);
            break; 
        }
        }

        lastSpawned = time;
    }
    sys->update();
}