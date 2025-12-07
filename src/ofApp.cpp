
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
	skybox.load("stars_dn.jpg");


	// camera

	// Example coordinates you got:
	camPositions.push_back(glm::vec3(0, 50, 20)); // Landing Zone Start (default 0)
	camPositions.push_back(glm::vec3(76.4916, 14.7818, 86.614)); // Landing Zone 1
	camPositions.push_back(glm::vec3(-86.0892, 40.9025, -51.3489)); // Landing Zone 2
	camPositions.push_back(glm::vec3(26.6946, 19.3444, -74.3588)); // Landing Zone 3
	// camera 4 onward will use onboard
	

	trackingCam.setPosition(0, 50, 20); // Landing Center Start
	trackingCam.lookAt(glm::vec3(0, 0, 0)); // initial target
	trackingCam.setNearClip(0.1);
	trackingCam.setFarClip(2000);



}
 
//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {
	
	PhysicsUpdate();
	
		if (!bLanderLoaded) return;

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();
		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

		colBoxList.clear();
		octree.intersect(bounds, octree.root, colBoxList);
	
		if (!colBoxList.empty()) {
			// Simple resolution: push up and stop vertical velocity
			glm::vec3 pos = lander.getPosition();
			if (lander.physics.vel.y < 0.0f) {
				pos.y += 0.01f;                 // small bump up
				lander.setPosition(pos.x, pos.y, pos.z);
				lander.physics.vel.y = 0.0f;    // stop falling
			}
			//		lander.physics.vel.y = 0.0f;
			//		bGrounded = true;
			//		cout << "bGrounded = true!" << endl;
		} else {
//			bGrounded = false;
			colBoxList.clear();
		}


		// tracking camera
		if (currentLandingCam < 4)
		{
			trackingCam.setPosition(camPositions[currentLandingCam]);
			trackingCam.lookAt(lander.getPosition());
		}
		// onboard camera
		else
		{
			glm::vec3 landerPos = lander.getPosition();
			trackingCam.setPosition(landerPos + glm::vec3(0, 10, -8));
			trackingCam.lookAt(landerPos + glm::vec3(0, 2, 0));
		}
		
		updateAltitudeTelemetry();


}
//--------------------------------------------------------------
void ofApp::draw() {


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



	ofPushMatrix();
	if (bWireframe) {                    // wireframe mode  (include axis)
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

				ofVec3f min = lander.getSceneMin() + lander.getPosition();
				ofVec3f max = lander.getSceneMax() + lander.getPosition();

				Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
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

	ofPopMatrix();
	cam.end();
	
	ofDisableDepthTest();
	glDepthMask(false);
	
	drawAltitudeTelemetry();
	
	if (!bHide) {
	 gui.draw();
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
		cam.reset();
		break;
	case 's':
		savePicture();
		break;
	case 't':
		setCameraTarget();
		break;
	case 'u':
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
		break;
	case OF_KEY_DOWN:
		bMoveBackward = true;
		break;
	case OF_KEY_LEFT:
		bMoveLeft = true;
		break;
	case OF_KEY_RIGHT:
		bMoveRight = true;
		break;
	case 'q': // yaw left
		bYawLeft = true;
		break;
	case 'e': // yaw right
		bYawRight = true;
		break;
	case ' ':
		bMoveUp = true;
		break;
	case OF_KEY_SHIFT:
		bMoveDown = true;
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
	glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(x, y, 0));
	cout << "Clicked world coords: " << mouseWorld << endl;

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

		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}

void ofApp::PhysicsDebugSetup() {
	physicsGui.setup("Physics Debug");
	physicsGui.setPosition(10, 220);

	physicsGui.add(thrustSlider.setup("Thrust", 0.0f, 0.0f, 50.0f));
	physicsGui.add(thrustMaxSlider.setup("Thrust Max", 20.0f, 0.0f, 100.0f));
	physicsGui.add(dampingSlider.setup("Linear Damping", 0.99f, 0.80f, 1.0f));
	physicsGui.add(massSlider.setup("Mass", 0.8f, 0.1f, 10.0f));

//	physicsGui.add(angVelXSlider.setup("Ang Vel X", 0.0f, -180.0f, 180.0f));
//	physicsGui.add(angVelYSlider.setup("Ang Vel Y", 0.0f, -180.0f, 180.0f));
//	physicsGui.add(angVelZSlider.setup("Ang Vel Z", 0.0f, -180.0f, 180.0f));
//
//	physicsGui.add(torqueXSlider.setup("Torque X", 0.0f, -50.0f, 50.0f));
//	physicsGui.add(torqueYSlider.setup("Torque Y", 0.0f, -50.0f, 50.0f));
//	physicsGui.add(torqueZSlider.setup("Torque Z", 0.0f, -50.0f, 50.0f));

	physicsGui.add(rotDampingSlider.setup("Rot Damping", 0.99f, 0.80f, 1.0f));
}
void ofApp::PhysicsUpdate() {

	if (!bLanderLoaded) return;

	lander.physics.damping           = static_cast<float>(dampingSlider);
	lander.physics.mass              = static_cast<float>(massSlider);
	lander.physics.rotationalDamping = static_cast<float>(rotDampingSlider);

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
