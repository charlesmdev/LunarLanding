
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Octree Test - startup scene
// 
//
//  Student Name:   Charles Vincent Manaois
//  Date: 11/15/2025


#include "ofApp.h"
#include "Util.h"


//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
	levelColors.resize(10);
	for(int i = 0; i < 10; i++) {
		levelColors[i] = ofColor::fromHsb(i * 25, 255, 255); // Random hue shift colors, I used this rather than hardcoding colors.
	}

	
	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;
	bTerrainSelected = true;
//	ofSetWindowShape(1024, 768);
	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();

	// setup rudimentary lighting 
	//
	initLightingAndMaterials();

	//	mars.loadModel("geo/moon-houdini.obj");
	//  mars.loadModel("geo/mars-low-5x-v2.obj");

//	mars.loadModel("geo/maya-moon-combined.obj");

	mars.loadModel("geo/dev-space-moon-alt-v2-final.obj");


	mars.setScaleNormalization(false);

	PhysicsDebugSetup();
	gui.setup();
	gui.add(numLevels.setup("Number of Octree Levels", 1, 1, 10));
	gui.add(timingToggle.setup("Timing Info", false));
	bHide = false;

	//  Create Octree for testing.
	//
	buildStartTime = ofGetElapsedTimeMillis();

	// need this to combine meshes (terrain, flags, everything)
	for (int i = 0; i < mars.getNumMeshes(); i++) {
		combined.append(mars.getMesh(i));
	}

	octree.create(combined, 20);

	buildEndTime = ofGetElapsedTimeMillis();
	
	cout << "Octree Build Time: " << (buildEndTime - buildStartTime) << " ms" << endl;
	cout << "Number of Verts: " << mars.getMesh(0).getNumVertices() << endl;

	testBox = Box(Vector3(3, 3, 0), Vector3(5, 5, 2));


	// sky box
	skybox.load("images/stars_dn.jpg");


	// Camera

	// camera coordinates for landing zones
	camPositions.push_back(glm::vec3(0, 50, 20)); // Landing Zone Start (default 0)
	camPositions.push_back(glm::vec3(76.4916, 14.7818, 86.614)); // Landing Zone 1
	camPositions.push_back(glm::vec3(-86.0892, 40.9025, -51.3489)); // Landing Zone 2
	camPositions.push_back(glm::vec3(42.051, 20, -63.273)); // Landing Zone 3
	// camera 4 onward will use onboard
	
	trackingCam.setPosition(0, 50, 20); // Landing Center Start
	trackingCam.lookAt(glm::vec3(0, 0, 0)); // initial target
	trackingCam.setNearClip(0.1);
	trackingCam.setFarClip(2000);


	// Lighting

	// Environmental lighting
	keyLight.setup();
	keyLight.enable();
	keyLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	keyLight.setDiffuseColor(ofFloatColor(1, 1, 1));
	keyLight.setSpecularColor(ofFloatColor(1, 1, 1));
	keyLight.rotate(-90, ofVec3f(1, 0, 0));
	keyLight.setPosition(0, 40, 0);


	// Landing Zone 1
	keyLight1.setup();
	keyLight1.enable();
	keyLight1.setSpotlight();
	keyLight1.setSpotlightCutOff(25); // angle of the cone
	keyLight1.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	keyLight1.setDiffuseColor(ofFloatColor(1, 1, 1));
	keyLight1.setSpecularColor(ofFloatColor(1, 1, 1));
	keyLight1.rotate(-90, ofVec3f(1, 0, 0));
	keyLight1.setPosition(32.226, 25, 45.588); // NOTE: Use this position to get the coords for landing zone 1

	fillLight1.setup();
	fillLight1.enable();
	fillLight1.setScale(0.05);
	fillLight1.setAttenuation(1, 0.001, 0.001);
	fillLight1.setAmbientColor(ofFloatColor(0.05, 0.05, 0.05)); 
	fillLight1.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0)); 
	fillLight1.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
	fillLight1.setPosition(32.226, 25, 45.588);
	fillLight1.lookAt(glm::vec3(32.226, 25, 45.588));


	// Landing Zone 2
	keyLight2.setup();
	keyLight2.enable();
	keyLight2.setSpotlight();
	keyLight2.setSpotlightCutOff(20); // angle of the cone
	keyLight2.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	keyLight2.setDiffuseColor(ofFloatColor(1, 1, 1));
	keyLight2.setSpecularColor(ofFloatColor(1, 1, 1));
	keyLight2.rotate(-90, ofVec3f(1, 0, 0));
	keyLight2.setPosition(-62.6381, 50, -33.1133); // NOTE: Use this position to get the coords for landing zone 2

	fillLight2.setup();
	fillLight2.enable();
	fillLight2.setScale(0.05);
	fillLight2.setAttenuation(1, 0.001, 0.001);
	fillLight2.setAmbientColor(ofFloatColor(0.05, 0.05, 0.05));
	fillLight2.setDiffuseColor(ofFloatColor(2.0, 2.0, 2.0));
	fillLight2.setSpecularColor(ofFloatColor(2.0, 2.0, 2.0));
	fillLight2.setPosition(-62.6381, 50, -33.1133);
	fillLight2.lookAt(glm::vec3(-62.6381, 50, -33.1133));


	// Landing Zone 3
	keyLight3.setup();
	keyLight3.enable();
	keyLight3.setSpotlight();
	keyLight3.setSpotlightCutOff(25); // angle of the cone
	keyLight3.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	keyLight3.setDiffuseColor(ofFloatColor(1, 1, 1));
	keyLight3.setSpecularColor(ofFloatColor(1, 1, 1));
	keyLight3.rotate(-90, ofVec3f(1, 0, 0));
	keyLight3.setPosition(40, 19.3444, -60.3588); // NOTE: Use this position to get the coords for landing zone 3

	fillLight3.setup();
	fillLight3.enable();
	fillLight3.setScale(0.05);
	fillLight3.setAttenuation(1, 0.001, 0.001);
	fillLight3.setAmbientColor(ofFloatColor(0.05, 0.05, 0.05));
	fillLight3.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
	fillLight3.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
	fillLight3.setPosition(46.6946, 19.3444, -74.3588);
	fillLight3.lookAt(glm::vec3(-86.0892, 40.9025, -20.3489));

	// Lander light
	landerLight.setup();
	landerLight.enable();
	landerLight.setPointLight(); 
	landerLight.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
	landerLight.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
	landerLight.setAttenuation(1.0, 0.01, 0.001);
	
	setupLandingZones();

	// sound

	// rocket thrust
	engineSound.load("sounds/rocket-engine.mp3");
	engineSound.setVolume(0.05); // quite loud otherwise
	engineSound.setLoop(true);

	// crash warning
	crashWarning.load("sounds/crash-warning.mp3");
	crashWarning.setVolume(0.05); // quite loud otherwise
	crashWarning.setLoop(true);

	// explosion sound
	explosionSound.load("sounds/explosion.mp3");
	explosionSound.setVolume(0.02);



	// load model early
	lander.loadModel("geo/dev-space-lander.obj");
	lander.setScaleNormalization(false);
	lander.setPosition(-30, 90, 60);
	lander.setRotation(glm::vec3(0, 180, 0));

	bLanderLoaded = true;
	for (int i = 0; i < lander.getMeshCount(); i++) {
		bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
	}

	// have landing cam onboard shown immedietely first
	currentLandingCam = 4;
	useTrackingCam = true;
	
	particleSetup();

}
 
//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {
	if (!bLanderLoaded) return;
	PhysicsUpdate();
	setupLandingZones(); // IMPORTANT FOR DEBUGGING DELETE LATER!!!
	Box bounds = computeLanderBounds();
	colBoxList.clear();
	octree.intersect(bounds, octree.root, colBoxList);

	if (!colBoxList.empty() && !lander.isCrashed()) {
		glm::vec3 n(0, 1, 0);
		float vRel = glm::dot(lander.physics.vel, n);

		if (vRel < 0.0f) {                // moving into ground
			float impactSpeed = -vRel;    // positive

//				cout << "Impact speed: " << impactSpeed
//					 << "  threshold: " << lander.crashSpeedThreshold << endl;

			if (impactSpeed > lander.crashSpeedThreshold) {
				// ----- crash -----
				lander.setCrashed(true);
				lander.physics.vel = glm::vec3(0);

				glm::vec3 pos = lander.getPosition();
				pos += n * 0.01f;
				lander.setPosition(pos.x, pos.y, pos.z);
				explosionSound.play(); // play explosion on death
				roundOver = true;

//					cout << "CRASH!" << endl;
			}
			else {
				// ----- safe bounce -----
				float e = lander.restitution;
				float vRelAfter = -e * vRel;
				float deltaV    = vRelAfter - vRel;
				float j = lander.physics.mass * deltaV;

				lander.physics.vel += (j / lander.physics.mass) * n;

				glm::vec3 pos = lander.getPosition();
				pos += n * 0.01f;
				lander.setPosition(pos.x, pos.y, pos.z);

//					cout << "Safe landing." << endl;
				
				// Handles landing zone logic
				Box landerBox = computeLanderBounds();

				lastLandingWasSuccess = false;
				lastLandingWasCrash = false;

				bool onZone = false;
				for (int i = 0; i < 3; ++i) {
					if (landingZones[i].overlaps(landerBox)) {
						onZone = true;
						break;
					}
				}
				
//				bool outOfFuel = !lander.hasFuel();

				// Case 1: gentle landing inside a zone
				if (onZone && !roundOver) {
					score += 1;
					successfulLandings += 1;
					lastLandingWasSuccess = true;
					roundOver = true;
					cout << "SUCCESSFUL LANDING! Score = " << score << endl;
				}
				// Case 2: gentle landing outside any zone
				else if (!onZone && !roundOver) {
					successfulLandings += 1;
					lastLandingWasCrash = true;
					roundOver = true;
					cout << "CRASH LANDING (no fuel, no score)." << endl;
				}
			}
		}
	}
	else {
		colBoxList.clear();
	}


	// tracking camera
	if (currentLandingCam < 4) // landing zones
	{
		trackingCam.setPosition(camPositions[currentLandingCam]);
		trackingCam.lookAt(lander.getPosition());
	}
	// onboard camera

	else if (currentLandingCam == 4) // third person
	{
		glm::vec3 landerPos = lander.getPosition();
		trackingCam.setPosition(landerPos + glm::vec3(0, 10, 12));
		trackingCam.lookAt(landerPos + glm::vec3(0, 2, 0));
	}

	else if (currentLandingCam == 5) // top down
	{
		glm::vec3 landerPos = lander.getPosition();
		trackingCam.setPosition(landerPos + glm::vec3(0, 50, 0));
		trackingCam.lookAt(landerPos + glm::vec3(0, 0, -1));
	}

	else if (currentLandingCam == 6) // below lander cam
	{
		glm::vec3 landerPos = lander.getPosition();
		trackingCam.setPosition(landerPos + glm::vec3(0, 1, 5));
		trackingCam.lookAt(landerPos + glm::vec3(0, 0, 0));
	}

	// lander light
	glm::vec3 landerPos = lander.getPosition();
	landerLight.setPosition(landerPos + glm::vec3(0, 5, 0)); // offset above lander

	// sound

	// check if its moving first and also if it hasn't crashed
	bool isMoving = ((bMoveForward || bMoveBackward || bMoveLeft || bMoveRight || bMoveUp || bMoveDown || bYawLeft || bYawRight) && lander.crashed == false);

	// check if it crashed to turn off crashwarning
	if (lander.crashed)
	{
		if (crashWarning.isPlaying())
		{
			crashWarning.stop(); // stop warning loop sound
		}
		engineSound.stop(); // stop engine too
	}

	// if lander has fuel
	else if (lander.hasFuel())
	{

		// if its moving, play sound
		if (isMoving && !engineSound.isPlaying())
		{
			engineSound.play();
		}

		// turn off it not moving
		else if (!isMoving && engineSound.isPlaying())
		{
			engineSound.stop();
		}
	}

	// no fuel
	else
	{
		// turn off engine sounds
		engineSound.stop();

		// play warning sounds on loop
		if (!crashWarningPlayed)
		{
			crashWarning.play();
			crashWarningPlayed = true;
		}
	}
	
	thrustEmitterUpdate();
	updateAltitudeTelemetry();


}
//--------------------------------------------------------------
void ofApp::draw() {

	// draw skybox
	ofDisableDepthTest();
	ofSetColor(255); // reset color
	skybox.draw(0, 0, ofGetWidth(), ofGetHeight());
	ofEnableDepthTest();
	

//	glDepthMask(false);
////	if (!bHide) gui.draw();
//	if (!bHide) {
//		  gui.draw();
//		  if (bShowPhysicsGui) {
//			  physicsGui.draw();
//		  }
//	  }
//	glDepthMask(true);

	

	if (useTrackingCam) {
		trackingCam.begin();
	}

	else{
		cam.begin();
	}

	// draw lighting
	// NOTE: only uncomment thisif you want to see the physical light

	/*keyLight.draw();*/

	/*keyLight1.draw();
	fillLight1.draw();

	keyLight2.draw();
	fillLight2.draw();

	keyLight3.draw();
	fillLight3.draw();
	*/
	




	ofPushMatrix();
	if (bWireframe) {  // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		mars.drawWireframe();
		if (bLanderLoaded) {
			lander.drawWireframe();
			if (!bTerrainSelected) drawAxis(lander.getPosition());
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		mars.drawFaces();
		ofMesh mesh;
		if (bLanderLoaded) {
			lander.drawFaces();
			drawThruster();
			
			if (!bTerrainSelected) drawAxis(lander.getPosition());
			if (bDisplayBBoxes) {
				ofNoFill();
				ofSetColor(ofColor::white);
				for (int i = 0; i < lander.getNumMeshes(); i++) {
					ofPushMatrix();
					ofMultMatrix(lander.getModelMatrix());
					ofRotate(-90, 1, 0, 0);
					Octree::drawBox(bboxList[i]);
					ofPopMatrix();
				}
			}

			if (bLanderSelected) {
				Box bounds = computeLanderBounds();
				ofSetColor(ofColor::white);
				ofNoFill();

				if (bInDrag) {
					ofSetColor(ofColor::red);
					ofNoFill();
				}
				Octree::drawBox(bounds);

			

				// draw colliding boxes
				//
				ofSetColor(ofColor::lightBlue);
				for (int i = 0; i < colBoxList.size(); i++) {
					Octree::drawBox(colBoxList[i]);
				}
			}
		}
	}
	if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));



	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		mars.drawVertices();
	}

	// highlight selected point (draw sphere around selected point)
	//
	if (bPointSelected) {
		ofSetColor(ofColor::blue);
		ofDrawSphere(selectedPoint, .1);
	}


	// recursively draw octree
	//
	ofDisableLighting();
	int level = 0;
	//	ofNoFill();

	if (bDisplayLeafNodes) {
		octree.drawLeafNodes(octree.root);
		cout << "num leaf: " << octree.numLeaf << endl;
    }
	else if (bDisplayOctree) {
		ofNoFill();
//		ofSetColor(ofColor::white);
		octree.draw(numLevels, 0, levelColors);
	}

	// if point selected, draw a sphere
	//
	if (pointSelected) {
		ofVec3f p = octree.mesh.getVertex(selectedNode.points[0]);
		ofVec3f d = p - cam.getPosition();
		ofSetColor(ofColor::lightGreen);
		ofDrawSphere(p, .02 * d.length());
	}
	
	if (bShowAltitudeRay && hasAltitudeHit) {
		ofSetColor(ofColor::yellow);
		ofSetLineWidth(2.0f);

		glm::vec3 p0(altitudeRayOrigin.x(),
					 altitudeRayOrigin.y(),
					 altitudeRayOrigin.z());

		glm::vec3 p1(altitudeRayOrigin.x(),
					 altitudeGroundY,
					 altitudeRayOrigin.z());

		ofDrawLine(p0, p1);
	}
	
	drawLandingZones();

	ofPopMatrix();
	cam.end();
	
	ofDisableDepthTest();
	glDepthMask(false);
	
	drawAltitudeTelemetry();
	drawFuel();
	drawScore();
	drawEndRoundMessage();
	
	if (!bHide) {
	 gui.draw();
	if (bShowThrustGui) {
		thrustGui.draw();
	}
	if (bShowPhysicsGui) {
		 physicsGui.draw();
	 }
	}

	glDepthMask(true);
}


// 
// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
	

	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}


void ofApp::keyPressed(int key) {

	switch (key) {
	case 'B':
	case 'b':
		bDisplayBBoxes = !bDisplayBBoxes;
		break;
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		break;
	case 'L':
	case 'l':
		bDisplayLeafNodes = !bDisplayLeafNodes;
		break;
	case 'O':
	case 'o':
		bDisplayOctree = !bDisplayOctree;
		break;
	case 'r':
		// changing [r]eset button so it'll reset to lander's last position, while enabling easy free cam
		// add a bit of an offset from lander position
		cam.setPosition(lander.getPosition() + glm::vec3 (0, 15, -20));
		cam.lookAt(lander.getPosition() + glm::vec3(0, 2, 0)); 
		break;
	case 's':
		savePicture();
		break;
	case 't':
		setCameraTarget();
		break;
	case 'u':
		if (lander.isCrashed() || roundOver || lastLandingWasSuccess || lastLandingWasCrash) {
			roundOver = false;
			reloadModel();
			lastLandingWasSuccess = false;
			lastLandingWasCrash   = false;
		}
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 'w':
		toggleWireframeMode();
		break;
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_DEL:
		break;
	case 'x':
		bResolvingCollision = true;
		break;
	case OF_KEY_UP:
		bMoveForward = true;
		thrusterActivated = true;
		break;
	case OF_KEY_DOWN:
		bMoveBackward = true;
		thrusterActivated = true;
		break;
	case OF_KEY_LEFT:
		bMoveLeft = true;
		thrusterActivated = true;
		break;
	case OF_KEY_RIGHT:
		bMoveRight = true;
		thrusterActivated = true;
		break;
	case 'q':
		bYawLeft = true;
		thrusterActivated = true;
		break;
	case 'e':
		bYawRight = true;
		thrusterActivated = true;
		break;
	case ' ':
		bMoveUp = true;
		thrusterActivated = true;
		break;
	case OF_KEY_SHIFT:
		bMoveDown = true;
		thrusterActivated = true;
		break;
	case '`':
		useTrackingCam = !useTrackingCam; // toggle tracking camera
		break;
	case '1':
		currentLandingCam = 1;
		break;
	case '2':
		currentLandingCam = 2;
		break;
	case '3':
		currentLandingCam = 3;
		break;
	case '4':
		currentLandingCam = 4;
		break;
	case '5':
		currentLandingCam = 5;
		break;
	case '6':
		currentLandingCam = 6;
		break;
	case 'a':
		bShowAltitudeHUD = !bShowAltitudeHUD;
		cout << "Altitude HUD: " << (bShowAltitude ? "ON" : "OFF") << endl;
		break;
	case 'z':   
		bShowAltitudeRay = !bShowAltitudeRay;
		cout << "Altitude Ray: " << (bShowAltitudeRay ? "ON" : "OFF") << endl;
		break;
	default:
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {
	switch (key) {
	case OF_KEY_UP:
		bMoveForward = false;
		break;
	case OF_KEY_DOWN:
		bMoveBackward = false;
		break;
	case OF_KEY_LEFT:
		bMoveLeft = false;
		break;
	case OF_KEY_RIGHT:
		bMoveRight = false;
		break;
	case 'q':
		bYawLeft = false;
		break;
	case 'e':
		bYawRight = false;
		break;
	case ' ':
		bMoveUp = false;
		break;
	case OF_KEY_SHIFT:
		bMoveDown = false;
		break;
	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	default:
		break;

	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

	
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	// if moving camera, don't allow mouse interaction
//
	if (cam.getMouseInputEnabled()) return;

	// if rover is loaded, test for selection
	//
	if (bLanderLoaded) {
		glm::vec3 origin = cam.getPosition();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		bool hit = bounds.intersect(Ray(Vector3(origin.x, origin.y, origin.z), Vector3(mouseDir.x, mouseDir.y, mouseDir.z)), 0, 10000);
		if (hit) {
			bLanderSelected = true;
			mouseDownPos = getMousePointOnPlane(lander.getPosition(), cam.getZAxis());
			mouseLastPos = mouseDownPos;
			bInDrag = true;
		}
		else {
			bLanderSelected = false;
		}
	}
	else {
		ofVec3f p;
		raySelectWithOctree(p);
	}

	// debug print to find some coords
	/*glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(x, y, 0));
	cout << "Clicked world coords: " << mouseWorld << endl;*/

}

bool ofApp::raySelectWithOctree(ofVec3f &pointRet) {
	
	searchStartTime = ofGetElapsedTimeMicros();
	
	ofVec3f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
		Vector3(rayDir.x, rayDir.y, rayDir.z));

	pointSelected = octree.intersect(ray, octree.root, selectedNode);
	
	searchEndTime = ofGetElapsedTimeMicros();
	
	if (timingToggle) {
		// Unfortunately I cannot do miliseconds since the value will show as 0, meaning the time is shorter than a milisecond.
		cout << "Ray Search Time: " << (searchEndTime - searchStartTime) << " Micro Sec at Cam Pos: " << cam.getPosition() << endl;
	}

	if (pointSelected) {
		pointRet = octree.mesh.getVertex(selectedNode.points[0]);

		// debug coordinates
		cout << "Intersection at: ("
			 << pointRet.x << ", "
			 << pointRet.y << ", "
			 << pointRet.z << ")"
			 << endl;
	}
	return pointSelected;
}




//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	if (bInDrag) {

		glm::vec3 landerPos = lander.getPosition();

		glm::vec3 mousePos = getMousePointOnPlane(landerPos, cam.getZAxis());
		glm::vec3 delta = mousePos - mouseLastPos;
	
		landerPos += delta;
		lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
		mouseLastPos = mousePos;

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

		colBoxList.clear();
		octree.intersect(bounds, octree.root, colBoxList);
	

		/*if (bounds.overlap(testBox)) {
			cout << "overlap" << endl;
		}
		else {
			cout << "OK" << endl;
		}*/


	}
	else {
		ofVec3f p;
		raySelectWithOctree(p);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bInDrag = false;
}



// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {

}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}



//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent2(ofDragInfo dragInfo) {

	ofVec3f point;
	mouseIntersectPlane(ofVec3f(0, 0, 0), cam.getZAxis(), point);
	if (lander.loadModel(dragInfo.files[0].string())) {
		lander.setScaleNormalization(false);
//		lander.setScale(.1, .1, .1);
	//	lander.setPosition(point.x, point.y, point.z);
		lander.setPosition(1, 1, 0);

		bLanderLoaded = true;
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		cout << "Mesh Count: " << lander.getMeshCount() << endl;
	}
	else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
	if (lander.loadModel(dragInfo.files[0].string())) {
		bLanderLoaded = true;
		lander.setScaleNormalization(false);
		lander.setPosition(0, 0, 0);
		cout << "number of meshes: " << lander.getNumMeshes() << endl;
		bboxList.clear();
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		//		lander.setRotation(1, 180, 1, 0, 0);

				// We want to drag and drop a 3D object in space so that the model appears 
				// under the mouse pointer where you drop it !
				//
				// Our strategy: intersect a plane parallel to the camera plane where the mouse drops the model
				// once we find the point of intersection, we can position the lander/lander
				// at that location.
				//

				// Setup our rays
				//
		glm::vec3 origin = cam.getPosition();
		glm::vec3 camAxis = cam.getZAxis();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		float distance;

		bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
		if (hit) {
			// find the point of intersection on the plane using the distance 
			// We use the parameteric line or vector representation of a line to compute
			//
			// p' = p + s * dir;
			//
			glm::vec3 intersectPoint = origin + distance * mouseDir;

			// Now position the lander's origin at that intersection point
			//
			glm::vec3 min = lander.getSceneMin();
			glm::vec3 max = lander.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			lander.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);

			// set up bounding box for lander while we are at it
			//
			landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		}
	}


}

//  intersect the mouse ray with the plane normal to the camera 
//  return intersection point.   (package code above into function)
//
glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
	// Setup our rays
	//
	glm::vec3 origin = cam.getPosition();
	glm::vec3 camAxis = cam.getZAxis();
	glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;

	bool hit = glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		glm::vec3 intersectPoint = origin + distance * mouseDir;

		cout << intersectPoint << endl;
		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}

void ofApp::PhysicsDebugSetup() {
	physicsGui.setup("Physics Debug");
	physicsGui.setPosition(10, 220);

	physicsGui.add(thrustSlider.setup("Thrust", 41.0f, 0.0f, 100.0f));
	physicsGui.add(thrustMaxSlider.setup("Thrust Max", 102.0f, 0.0f, 200.0f));
	physicsGui.add(dampingSlider.setup("Linear Damping", 0.9987f, 0.80f, 1.0f));
	physicsGui.add(massSlider.setup("Mass", 10.0f, 0.1f, 100.0f));

//	physicsGui.add(angVelXSlider.setup("Ang Vel X", 0.0f, -180.0f, 180.0f));
//	physicsGui.add(angVelYSlider.setup("Ang Vel Y", 0.0f, -180.0f, 180.0f));
//	physicsGui.add(angVelZSlider.setup("Ang Vel Z", 0.0f, -180.0f, 180.0f));
//
//	physicsGui.add(torqueXSlider.setup("Torque X", 0.0f, -50.0f, 50.0f));
//	physicsGui.add(torqueYSlider.setup("Torque Y", 0.0f, -50.0f, 50.0f));
//	physicsGui.add(torqueZSlider.setup("Torque Z", 0.0f, -50.0f, 50.0f));

	physicsGui.add(rotDampingSlider.setup("Rot Damping", 0.99f, 0.80f, 1.0f));
	
	physicsGui.add(fuelMaxSlider.setup("Fuel Max", 300.0f, 0.0f, 500.0f));
	physicsGui.add(fuelSlider.setup("Fuel",       300.0f, 0.0f, 500.0f));
	
	physicsGui.add(restitutionSlider.setup("Restitution", 0.3f, 0.0f, 1.0f));
	physicsGui.add(crashSpeedSlider.setup("Crash Speed", 1.4f, 0.0f, 20.0f));
	
	physicsGui.add(landingZoneHalfX.setup("LZ Half X", 5.0f, 0.5f, 50.0f));
	physicsGui.add(landingZoneHalfY.setup("LZ Half Y", 2.0f, 0.1f, 50.0f));
	physicsGui.add(landingZoneHalfZ.setup("LZ Half Z", 5.0f, 0.5f, 50.0f));
	
	// Thrust debug GUI
	thrustGui.setup("Thrust Debug");
	thrustGui.setPosition(10, 420);

	thrustGui.add(thrustLifeSlider.setup("Life (s)", 1.0f, 0.1f, 5.0f));
	thrustGui.add(thrustRateSlider.setup("Rate (pps)", 60.0f, 1.0f, 200.0f));
	thrustGui.add(thrustSpeedSlider.setup("Speed", 80.0f, 10.0f, 300.0f));
	thrustGui.add(thrustSizeSlider.setup("Size", 10.0f, 1.0f, 40.0f));
	thrustGui.add(thrustYOffsetSlider.setup("Y Offset", 1.0f, -5.0f, 5.0f));


	// adding reload model
	physicsGui.add(modelReloadButton.setup("Reload model"));
	modelReloadButton.addListener(this, &ofApp::reloadModel);

}
void ofApp::PhysicsUpdate() {

	if (!bLanderLoaded) return;
	if (lander.isCrashed()) {
		lander.physics.vel = glm::vec3(0);
		return;
	}
	lander.fuelMax = (float)fuelMaxSlider;
	lander.fuel = ofClamp((float)fuelSlider, 0.0f, lander.fuelMax);

	lander.physics.damping           = static_cast<float>(dampingSlider);
	lander.physics.mass              = static_cast<float>(massSlider);
	lander.physics.rotationalDamping = static_cast<float>(rotDampingSlider);
	
	lander.restitution = (float)restitutionSlider;
	lander.crashSpeedThreshold = (float)crashSpeedSlider;


	// Gravity
	glm::vec3 gravity(0, -1.62f * lander.physics.mass, 0);
	lander.physics.addForce(gravity);
//	if (!bGrounded) {
//		cout << "Not grounded!" << endl;
//		glm::vec3 gravity(0, -1.62f * lander.physics.mass, 0);
//		lander.physics.addForce(gravity);
//	}

	// Direction basis from heading
	glm::vec3 fwd  = lander.getForwardDir();
	glm::vec3 right = lander.getRightDir();
	glm::vec3 up    = lander.getUpDir();

	float moveThrust = static_cast<float>(thrustSlider);
	
	bool thrustersRequested =
			bMoveForward || bMoveBackward ||
			bMoveRight   || bMoveLeft     ||
			bMoveUp      || bMoveDown     ||
			bYawLeft     || bYawRight;

	bool thrustersActive = thrustersRequested && lander.hasFuel();
	
	// start when active, stop when not
	if (thrustersActive && !thrustEmitter.started) {
		thrustEmitter.start();
	} else if (!thrustersActive && thrustEmitter.started) {
		thrustEmitter.stop();
	}

	// always update, it will only spawn when started
	thrustEmitter.update();

	if (thrustersActive) {
		if (bMoveForward) {
			lander.physics.addForce(fwd * moveThrust);
		}
		if (bMoveBackward) {
			lander.physics.addForce(-fwd * moveThrust);
		}
		if (bMoveRight) {
			lander.physics.addForce(right * moveThrust);
		}
		if (bMoveLeft) {
			lander.physics.addForce(-right * moveThrust);
		}
		if (bMoveUp) {
			lander.physics.addForce(up * moveThrust);
		}
		if (bMoveDown) {
			lander.physics.addForce(-up * moveThrust);
		}
		float yawTorque = 30.0f;
		if (bYawLeft) {
			lander.physics.addTorque(glm::vec3(0, yawTorque, 0));   // +Y rotation
		}
		if (bYawRight) {
			lander.physics.addTorque(glm::vec3(0, -yawTorque, 0));  // -Y rotation
		}
		
	  float dt = ofGetLastFrameTime();  // seconds
	  float thrustFactor = moveThrust / std::max(0.01f, (float)thrustMaxSlider);
	  float burn = lander.fuelBurnRate * thrustFactor * dt;
	  lander.fuel = std::max(0.0f, lander.fuel - burn);
	}
	
	fuelSlider = lander.fuel;

	lander.updatePhysics();
}

void ofApp::updateAltitudeTelemetry() {
	hasAltitudeHit = false;
	altitudeAGL    = 0.0f;

	if (!bLanderLoaded) return;

	glm::vec3 landerPos = lander.getPosition();
	altitudeRayOrigin = Vector3(landerPos.x, landerPos.y + 0.1f, landerPos.z);
	altitudeRayDir    = Vector3(0.0f, -1.0f, 0.0f);
	Ray ray(altitudeRayOrigin, altitudeRayDir);

	TreeNode hitNode;
	bool hit = octree.intersect(ray, octree.root, hitNode);
	if (hit) {
		hasAltitudeHit  = true;
		const Box &box  = hitNode.box;
		altitudeGroundY = box.max().y();
		altitudeAGL     = altitudeRayOrigin.y() - altitudeGroundY;
	}
}

void ofApp::drawAltitudeTelemetry() {
	ofDisableDepthTest();

	if (bShowAltitudeHUD) {
		ofSetColor(255);
		std::string msg;
		if (hasAltitudeHit) {
			msg = "Altitude (AGL): " + ofToString(altitudeAGL, 2) + " m";
		} else {
			msg = "Altitude (AGL): N/A";
		}
		ofDrawBitmapStringHighlight(msg, 20, 20);
	}
}

void ofApp::drawFuel() {
	if (!bShowAltitudeHUD) return;  // same toggle as altitude HUD

	ofSetColor(255);

	float fuelPct = 0.0f;
	if (lander.fuelMax > 0.0f) {
		fuelPct = (lander.fuel / lander.fuelMax) * 100.0f;
	}

	std::string fuelMsg =
		"Fuel: " + ofToString(lander.fuel, 1) +
		" (" + ofToString(fuelPct, 0) + "%)";

	// Draw under altitude text
	ofDrawBitmapStringHighlight(fuelMsg, 20, 40);
}

void ofApp::resetLander() {
	if (!bLanderLoaded) return;
	lander.setCrashed(false);
	lander.physics.vel    = glm::vec3(0);
	lander.fuel    = lander.fuelMax;
	fuelSlider     = lander.fuel;
	lander.setPosition(0, 10, 0);

	cout << "Lander reset." << endl;
}


void ofApp::setupLandingZones() {
	// Coordinates are based on the coords of the landing lights, relative to the map model
//	glm::vec3 zoneHalfSize(5.0f, 2.0f, 5.0f);
	glm::vec3 zoneHalfSize( // We will comment this out later, use this to find right dimensions for each landing zone.
		(float)landingZoneHalfX,
		(float)landingZoneHalfY,
		(float)landingZoneHalfZ
	);
	
	glm::vec3 zone1Size; // Modify these values during tuning.
	glm::vec3 zone2Size;
	glm::vec3 zone3Size;
	
	glm::vec3 zone1Center(32.226f, 0.5f, 45.588f);
	landingZones[0].set(zone1Center, zoneHalfSize); // zone 1

	glm::vec3 zone2Center(-62.6381f, 0.5f, -33.1133f);
	landingZones[1].set(zone2Center, zoneHalfSize); // zone 2

	glm::vec3 zone3Center(40.0f, 0.5f, -60.3588f);
	landingZones[2].set(zone3Center, zoneHalfSize); // zone 3

}

void ofApp::drawLandingZones() {
	ofNoFill();
	ofSetColor(ofColor::yellow);
	for (int i = 0; i < 3; ++i) {
		landingZones[i].draw();
	}
}

// Reloads model
void ofApp::reloadModel() {
	// load model early
	lander.setCrashed(false); // need this off to show lander again
	lander.loadModel("geo/dev-space-lander.obj");
	lander.setScaleNormalization(false);
	lander.setPosition(-30, 90, 60);
	lander.setRotation(glm::vec3(0, 180, 0));

	bLanderLoaded = true;
	for (int i = 0; i < lander.getMeshCount(); i++) {
		bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
	}

	// have landing cam onboard shown immedietely first
	currentLandingCam = 4;
	useTrackingCam = true;

	// reload fuel
	fuelSlider = (float)fuelMaxSlider;
}

Box ofApp::computeLanderBounds() {
	ofVec3f min = lander.getSceneMin() + lander.getPosition();
	ofVec3f max = lander.getSceneMax() + lander.getPosition();
	return Box(Vector3(min.x, min.y, min.z),
			   Vector3(max.x, max.y, max.z));
}

void ofApp::drawScore() {
	ofSetColor(255);
	std::string scoreMsg =
		"Score: " + ofToString(score) +
		"  Landings: " + ofToString(successfulLandings);
	ofDrawBitmapStringHighlight(scoreMsg, 20, 60);
}

void ofApp::drawEndRoundMessage() {
	if (!roundOver && !lander.isCrashed()) return;

	std::string msg;
	if (lander.isCrashed()) {
		msg = "DEATH! Press 'U' to restart.";
	} else if (lastLandingWasSuccess) {
		msg = "SUCCESSFUL LANDING! Press 'U' to restart.";
	} else if (lastLandingWasCrash) {
		msg = "CRASH LANDING. Press 'U' to restart.";
	} else {
		msg = "ROUND OVER. Press 'U' to restart.";
	}

	ofSetColor(255);
	int w = ofGetWidth();
	int h = ofGetHeight();
	int x = w / 2 - 200;
	int y = h / 2;
	ofDrawBitmapStringHighlight(msg, x, y);
}


void ofApp::particleSetup() {
	// --- particle explosion setup ---

	// load texture (soft round or smoke sprite)
	ofDisableArbTex(); // use normalized UVs
	if (!ofLoadImage(particleTex, "images/dot.png")) {
		cout << "Particle Texture File: images/dot.png not found" << endl;
	}

	// load particle shader (reuse CS134 shader)
	#ifndef TARGET_OPENGLES
	particleShader.load("shaders/shader");   
	#else
	particleShader.load("shaders_gles/shader");
	#endif

	// Thruster emitter config
	thrustEmitter.setEmitterType(DirectionalEmitter);
	thrustEmitter.setOneShot(false);
	thrustEmitter.setGroupSize(5);
	thrustEmitter.setRate(60.0f);
	thrustEmitter.setLifespan(1.0f);
	thrustEmitter.setParticleRadius(0.12f);
	thrustEmitter.setMass(0.5f);
	thrustEmitter.setDamping(0.90f);


}
void ofApp::drawThruster() {
	auto &particles = thrustEmitter.sys->particles;
	if (particles.empty()) return;

	vector<ofVec3f> points, sizes;
	points.reserve(particles.size());
	sizes.reserve(particles.size());
	for (auto &p : particles) {
		points.push_back(p.position);
//		sizes.push_back(ofVec3f(10.0f));
		sizes.push_back(ofVec3f(thrustSizeSlider));
	}
	int total = (int)points.size();
	thrustVbo.clear();
	thrustVbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
	thrustVbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);

	ofSetColor(150, 200, 255);          // bluish exhaust
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnablePointSprites();

	particleShader.begin();
	particleShader.setUniformTexture("tex0", particleTex, 0);

	thrustVbo.draw(GL_POINTS, 0, total);

	particleShader.end();
	ofDisablePointSprites();
	ofDisableBlendMode();
	ofEnableAlphaBlending();
}

void ofApp::thrustEmitterUpdate() {
	glm::vec3 landerPos = lander.getPosition();
	glm::vec3 up = lander.getUpDir();

//	glm::vec3 nozzlePos = landerPos - up * 0.1f + glm::vec3(0, 1.0f, 0);
//	thrustEmitter.setPosition(nozzlePos.x, nozzlePos.y, nozzlePos.z);
//
//	ofVec3f exhaustDir(-up.x, -up.y, -up.z);
//	exhaustDir.normalize();
//	thrustEmitter.setVelocity(exhaustDir * 80.0f);
	

	 glm::vec3 nozzlePos = landerPos - up * 0.1f + glm::vec3(0, (float) thrustYOffsetSlider, 0);
	 thrustEmitter.setPosition(nozzlePos.x, nozzlePos.y, nozzlePos.z);
	
	 ofVec3f exhaustDir(-up.x, -up.y, -up.z);
	 exhaustDir.normalize();
	 thrustEmitter.setV3elocity(exhaustDir * thrustSpeedSlider);

	 thrustEmitter.setLifespan(thrustLifeSlider);
	 thrustEmitter.setRate(thrustRateSlider);
}
