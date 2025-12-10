//#include "Player.h"
//#include "HeadingVector.h" 
//#include "utils/Helpers.h"
//
//Player::Player() : col(this, 15.0f) {
//    this->vertices = playerVerts;
//    // Write methods for physics componenets to initialize dampening, rotdampening, force, rotforce, mass, and maybe velocity. Add setters and getters for physic components.
//
//    gun.setEmitterType(DirectionalEmitter);
//    gun.setRate(10);             
//    gun.setParticleRadius(2);    
//    gun.setLifespan(1.0f);        
//    gun.setVelocity(ofVec3f(0, -600, 0)); 
//    gun.visible = false;
//
//    explosion.setEmitterType(RadialEmitter);
//    explosion.setRate(50);              
//    explosion.setParticleRadius(3);     
//    explosion.setLifespan(1.5f);        
//    explosion.visible = true;
//    explosion.started = false;      
//}
//
//void Player::draw() {
//    if(lives <= 0) return;
//    if(!exploding) {
//        col.setRadius(15.0f);
//        ofPushMatrix(); 
//        ofMultMatrix(getTransform());
//        this->polyShape.clear();
//        
//        for(auto& vertice : vertices) {
//            this->polyShape.addVertex(vertice); 
//        }
//
//        this->polyShape.close();
//        this->polyShape.draw();
//
//        // if (toggleHeading == true) 
//        this->heading->draw(); // For heading vector
//        
//        ofPopMatrix();
//        gun.draw(); // draw method outside of Matrix since putting it inside will not ...
//        if(showBoundingCircle) ofDrawCircle(position.x, position.y, col.getRadius()); // To check circle bounding box.
//    }
//    if(exploding) {
//        explosion.draw();
//    }
//}
//
//void Player::move(glm::vec3 thrust) {
//    if(lives <= 0) return;
//    if (exploding) { // If dying, don't move
//        explosion.sys->update();
//        updateExplosion();
//        return;
//    }
//    playerPhysics.integrate(*this, thrust);
//    //checkBounds();
//
//    glm::vec3 pos = getPosition(); 
//    Helpers::wrapAround(pos);
//    setPosition(pos);
//
//    gun.sys->update(); 
//    updateGun();
//
//    explosion.sys->update();
//    updateExplosion();
//}
//
//void Player::rotate(float torque) {
//    playerRotPhysics.torque = torque;
//    playerRotPhysics.integrate(*this, glm::vec3(0,0,0));
//}
//
//void Player::updateGun() {
//    // Makes the gun follow the player's nose, update in the future.
//    if(exploding) return;
//
//    glm::vec3 forward = heading->getForward();
//    glm::vec3 gunPos = getPosition() + forward * 20.0f; 
//
//    gun.position = ofVec3f(gunPos.x, gunPos.y, gunPos.z);
//    gun.velocity = ofVec3f(forward.x * 1500, forward.y * 1500, 0); 
//
//    // if(!gun.started) gun.start(); // Used this for continous shooting, decided to add spacebar.
//    if(gun.started) gun.update();  
//}
//
//void Player::updateExplosion() {
//    if (!exploding) return;  
//	    explosion.update();
//    if (ofGetElapsedTimeMillis() - explosionStartTime > 2000) { 
//        exploding = false;
//    }
//}
//
//void Player::fire() {
//    if(!gun.started) gun.start(); 
//}
//
//void Player::stopFire() {
//    if(gun.started) gun.stop();
//}
//
//// This is for drawing the heading vector, pretty complicated I know lol
//glm::vec3 Player::getCenterPoint() {
//    float xCentroid = 0.0;
//    float yCentroid = 0.0;
//    for(auto& vertice : vertices) {
//        xCentroid += vertice.x;
//        yCentroid += vertice.y;
//    }
//    return glm::vec3(xCentroid / 3, yCentroid /3, 0.0f);
//}
//float Player::getHeight() {
//    float minY = vertices[0].y, maxY = vertices[0].y;
//    for(auto& v : vertices) {
//        if(v.y < minY) minY = v.y;
//        if(v.y > maxY) maxY = v.y;
//    }
//    return maxY - minY;
//}
//// ^^ For heading Vector
//
//void Player::die() {
//    lives--;
//    stopFire();
//    gun.sys->particles.clear();
//
//    exploding = true;
//    explosionStartTime = ofGetElapsedTimeMillis();
//    
//    explosion.position = ofVec3f(position.x, position.y, position.z);
//    explosion.start();  // RadialEmitter will spawn 30 particles
//    
//    col.setRadius(0);
//}
