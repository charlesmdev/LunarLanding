// #include "ofApp.h"
// //#include "paraLine.h"
// //#include "Shape.h"
// //#include "CircleShape.h"
// //#include "TriangleShape.h"
// #include "HeadingVector.h"
// #include "shapes/Player.h"
// //#include "Testing.h"
// #include "shapes/Asteroid.h"
// #include <map>
// #include <memory>
// #include <vector>
// #include <tuple>
// using namespace std;

// ofTrueTypeFont font;
// ofTrueTypeFont gameOverFont;
// ofSoundPlayer shootSound;
// ofSoundPlayer thrustSound;
// ofSoundPlayer explosionSound;
// ofImage backgroundImage;

// bool isDragging = false;
// bool ctrlLeftBind = false;
// bool startGame = false;
// bool restartGame = false;

// float movementSpeed = 1000.0f;
// float torqueSpeed = 300.0f;
// float playerTorque = 0;
// float asteroidTorque = 100.0f;
// map<int, bool> keyState;

// int numAsteroids = 2; // initial number of asteroid;
// int minAsteroids = 1;
// int numOfSplits = 4; // Should be 4

// void ofApp::setup(){
// 	ofSetLineWidth(5);
// 	ofNoFill();
// 	ofSetFrameRate(60); // Lock fps when full screening things go faster >:o
// 	// Perhaps implement a way to set damping and other variables here.
// 	// playerShape.setPosition(glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0.0f));
// 	ofSetBackgroundColor(ofColor::black);
// 	font.load("cour.ttf", 20); 
// 	gameOverFont.load("cour.ttf", 40);

// 	soundSetup();
// 	panelSetup();
// 	gameSetup();

// 	backgroundImage.load("images/Spacebackround.jpg");
// }

// void ofApp::update(){
// 	if (!startGame) return; // Without this bool, the update function initializes too fast causing position and velocity to be nan values and the player to not move.
// 	// cout << "FPS: " << ofGetFrameRate() << " | dt: " << (1.0f/ofGetFrameRate()) << endl;
// 	playerShape.setSize(1.0);
// 	glm::vec3 thrust(0,0,0);
// 	playerTorque = 0;

//     bool isThrusting = false;
    
//     if (keyState[OF_KEY_UP]) {
//         thrust += playerShape.heading->getForward() * movementSpeed;
//         isThrusting = true;
//     }
//     if (keyState[OF_KEY_DOWN]) {
//         thrust -= playerShape.heading->getForward() * movementSpeed;
//         isThrusting = true;
//     }
//     if(keyState[OF_KEY_LEFT]) {
//         playerTorque = -torqueSpeed;
//         isThrusting = true;  // Add this - rotation also uses thrusters
//     }
//     if(keyState[OF_KEY_RIGHT]) {
//         playerTorque = torqueSpeed;
//         isThrusting = true;  // Add this - rotation also uses thrusters
//     }
// 	if (isThrusting && !isThrustPlaying) {
// 		thrustSound.play();
// 		isThrustPlaying = true;
//     } else if (!isThrusting && isThrustPlaying) {
//         thrustSound.stop();
//         isThrustPlaying = false;
//     }

// 	playerShape.move(thrust);
//     playerShape.rotate(playerTorque);

//     // cout << "Score: " << asteroidManager.getScore() << endl;
//     if (keyState[OF_KEY_SPACE]) {
//         playerShape.fire();
        
//         // Play sound every time (allows rapid fire sound)
//         if (!shootSound.isPlaying()) {
//             shootSound.play();
//         }
//     } else {
//         playerShape.stopFire();
//     }
//     asteroidManager.update();
//     colManager.checkCollisions();
// }

// void ofApp::handleCollision(Collision* a, Collision* b) {
// 	Shape* shapeA = a->getOwner();
// 	Shape* shapeB = b->getOwner();
// 	if (shapeA == &playerShape || shapeB == &playerShape) {
// 		// startGame = false;
// 		if (playerShape.exploding) return;
// 		playerShape.die();
// 		explosionSound.play(); 
// 		playerShape.setPosition(glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0.0f));
// 	}
// }
// void ofApp::handleParticleCollision(glm::vec3 particlePos, Collision* hit) {
//     // Find and remove the asteroid that was hit
// 	asteroidManager.removeAsteroid(hit);
// 	explosionSound.play();
// }

// void ofApp::draw(){
// 	backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
// 	if(!bHide) // Something wrong causing it to error log trying to load unallocated texture???
// 		panel.draw();
// 	playerShape.draw();
// 	asteroidManager.draw();
// 	ofDrawCircle(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f, 50);
// 	drawUI();
// }

// //--------------------------------------------------------------
// void ofApp::exit(){

// }

// //--------------------------------------------------------------
// void ofApp::keyReleased(int key){
// 	keyState[key] = false;
// 	if(ctrlLeftBind == true) {
// 		ctrlLeftBind = false;
// 		cout << "Control is not pressed." << endl;
// 	}
// }

// //--------------------------------------------------------------
// void ofApp::keyPressed(int key) {
// 	keyState[key] = true;
// 	if(key == OF_KEY_CONTROL) ctrlLeftBind = true;
// 	switch(key) {	
// 		case 'H':
// 		case 'h':
// 			bHide = !bHide;
// 			break;
// 		case 's':
// 			if(!startGame) startGame = true;
// 			else startGame = false;
// 			break;
// 		case 'r':
// 			gameSetup();
// 			startGame = false;
//             thrustSound.stop();
//             shootSound.stop();
//             isThrustPlaying = false;
// 			explosionSound.stop();
//             wasFiring = false;
// 			break;
// 	}
		
// }
// void ofApp::gameSetup() {

// 	asteroidManager.clear();
//     colManager.clear();

// 	playerShape.lives = 3;
// 	playerShape.setPosition(glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0.0f));
// 	colManager.addCollider(&playerShape.col);
// 	colManager.addParticleSystem(playerShape.gun.sys);
// 	asteroidManager.setup(numAsteroids, minAsteroids, numOfSplits, &colManager);

// 	/*This callback function handles collision of two objects using the collision manager, colManager. Collisions work by the player and asteroid object having a bounding circled shape and the collision manager constantly checking
// 	 object collision. Once collision is detected, a callback function, onCollision, is called which then passes through ofApp's handleCollision. I wanted collisions to be more abstract rather than hardcoded for future games.
// 	 */ 
// 	colManager.onCollision = [this](Collision* a, Collision* b) {
//         handleCollision(a, b);
//     };
//     colManager.onParticleCollision = [this](glm::vec3 pos, Collision* hit) {
//         handleParticleCollision(pos, hit);
//     };
// }
// void ofApp::soundSetup() {
// 	shootSound.load("sounds/fire.wav");
//     thrustSound.load("sounds/thrust.wav");
// 	explosionSound.load("sounds/bangMedium.wav");
    
//     thrustSound.setLoop(true);
//     thrustSound.setVolume(0.4f);
    
//     shootSound.setMultiPlay(true); 
//     shootSound.setVolume(0.4f);

// 	explosionSound.setMultiPlay(true); 
//     explosionSound.setVolume(0.6f);
// }
// void ofApp::panelSetup() {
// 	panel.setup();
// 	panel.add(toggleBoundingCircles.setup("Toggle Bounding Circles", false));
// 	toggleBoundingCircles.addListener(this, &ofApp::toggleBoundingShapeListener);

// 	panel.setPosition(200, 200);

// }
// void ofApp::toggleBoundingShapeListener() {
// 	if(!playerShape.showBoundingCircle) {
// 		playerShape.showBoundingCircle = true;
// 		asteroidManager.showAsteroidsBoundShapes = true;
// 	}
// 	else {
// 		playerShape.showBoundingCircle = false;
// 		asteroidManager.showAsteroidsBoundShapes = false;
// 	}
		
// }

// // I wanted to do something more for the UI but I am running out of time :( So this will have to do.

// void ofApp::drawUI() {
//     float margin = ofGetWidth() * 0.02; 
//     float startY = ofGetHeight() * 0.05; 
//     float lineHeight = 20;

//     string scoreText = "Score: " + ofToString(asteroidManager.getScore());
//     float scoreX = margin;
//     float scoreY = startY;
//     ofSetColor(100);
//     font.drawString(scoreText, scoreX + 3, scoreY + 3);
//     ofSetColor(255);
//     font.drawString(scoreText, scoreX, scoreY);

//     string livesText = "Lives: " + ofToString(playerShape.lives);
//     float livesX = margin;
//     float livesY = startY + lineHeight;
//     ofSetColor(100);
//     font.drawString(livesText, livesX + 3, livesY + 3);
//     ofSetColor(255);
//     font.drawString(livesText, livesX, livesY);

//     ofSetColor(200);
//     font.drawString("Controls:", margin, startY + lineHeight * 3);
//     font.drawString("Arrow Keys - Move", margin, startY + lineHeight * 4);
//     font.drawString("Space - Fire", margin, startY + lineHeight * 5);
//     font.drawString("S - Start/Pause", margin, startY + lineHeight * 6);
//     font.drawString("R - Restart", margin, startY + lineHeight * 7);
//     font.drawString("H - Toggle Debug Panel", margin, startY + lineHeight * 8);
// 	if (playerShape.lives <= 0) {
//     	string msg = "GAME OVER";
// 		float x = (ofGetWidth() - gameOverFont.stringWidth(msg)) / 2.0f;
//     	float y = (ofGetHeight() + gameOverFont.stringHeight(msg)) / 2.0f;
// 		ofSetColor(100);
// 		gameOverFont.drawString(msg, x + 3, y + 3);
//     	ofSetColor(255); 
//     	gameOverFont.drawString(msg, x, y);
// 	}

// }





// //--------------------------------------------------------------
// void ofApp::mouseReleased(int x, int y, int button){

// }

// //--------------------------------------------------------------
// void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

// }

// //--------------------------------------------------------------
// void ofApp::mouseEntered(int x, int y){

// }

// //--------------------------------------------------------------
// void ofApp::mouseExited(int x, int y){

// }

// //--------------------------------------------------------------
// void ofApp::windowResized(int w, int h){

// }

// //--------------------------------------------------------------
// void ofApp::gotMessage(ofMessage msg){

// }

// //--------------------------------------------------------------
// void ofApp::dragEvent(ofDragInfo dragInfo){ 

// }
// void ofApp::mouseMoved(int x, int y ){

// }

// //--------------------------------------------------------------
// void ofApp::mouseDragged(int x, int y, int button){


// }

// //--------------------------------------------------------------
// void ofApp::mousePressed(int x, int y, int button){



// }



