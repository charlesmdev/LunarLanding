#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include  "ofxAssimpModelLoader.h"
#include "Octree.h"
#include <glm/gtx/intersect.hpp>
#include "shapes/Lander.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent2(ofDragInfo dragInfo);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawAxis(ofVec3f);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
		void toggleSelectTerrain();
		void setCameraTarget();
		bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
		bool raySelectWithOctree(ofVec3f &pointRet);
		glm::vec3 getMousePointOnPlane(glm::vec3 p , glm::vec3 n);

		ofEasyCam cam;
		ofxAssimpModelLoader mars;
		ofLight light;
		Box boundingBox, landerBounds;
		Box testBox;
		vector<Box> colBoxList;
		bool bLanderSelected = false;
		Octree octree;
		TreeNode selectedNode;
		glm::vec3 mouseDownPos, mouseLastPos;
		bool bInDrag = false;


		ofxIntSlider numLevels;
		ofxPanel gui;

		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
		bool bHide;
		bool pointSelected = false;
		bool bDisplayLeafNodes = false;
		bool bDisplayOctree = false;
		bool bDisplayBBoxes = false;
		
		bool bLanderLoaded;
		bool bTerrainSelected;
	
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;

		vector<Box> bboxList;

		const float selectionRange = 4.0;
	
		bool bResolvingCollision = false;
		uint64_t buildStartTime, buildEndTime;
		uint64_t searchStartTime, searchEndTime;

		bool bTimingInfo;
		ofxToggle timingToggle;
	
		vector<ofColor> levelColors;

		// skybox
		ofImage skybox;

		// mesh
		ofMesh combined;
		Lander lander;
		bool bGrounded = false;
	
		ofxPanel physicsGui;

		// Lander physics parameters
		ofxFloatSlider thrustSlider;
		ofxFloatSlider thrustMaxSlider;
		ofxFloatSlider dampingSlider;
		ofxFloatSlider massSlider;

//		ofxFloatSlider angVelXSlider;
//		ofxFloatSlider angVelYSlider;
//		ofxFloatSlider angVelZSlider;
//
//		ofxFloatSlider torqueXSlider;
//		ofxFloatSlider torqueYSlider;
//		ofxFloatSlider torqueZSlider;

		ofxFloatSlider rotDampingSlider;

		bool bShowPhysicsGui = true;
		
		void PhysicsDebugSetup();
		void PhysicsUpdate();
	
		bool bMoveForward = false;
		bool bMoveBackward = false;
		bool bMoveLeft = false;
		bool bMoveRight = false;
		bool bMoveUp = false;
		bool bMoveDown = false;
		bool bYawLeft = false;
		bool bYawRight = false;


		// tracking cam
		ofCamera trackingCam;
		bool useTrackingCam;

		// landing zone cams
		vector<glm::vec3> camPositions;

		// cam indictator
		int currentLandingCam = 0; // 0 = start, 123 = landing zones




};
