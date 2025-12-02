//
//  Lander.h
//  LunarLanding
//
//  Created by Charles M. on 12/2/25.
//
#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Box.h"

class Lander {
public:
	Lander();

	bool loadModel(const std::string &path);
	void drawWireframe();
	void drawFaces();
	void setPosition(float x, float y, float z);
	ofVec3f getPosition() const;
	void setScaleNormalization(bool normalize);
	int getNumMeshes();          // remove const
	ofMesh getMesh(int i);       // remove const
	int getMeshCount();          // remove const
	ofVec3f getSceneMin();       // remove const
	ofVec3f getSceneMax();       // remove const
	ofMatrix4x4 getModelMatrix();// remove const


	// Add your extra attributes here
	// float fuelLevel;
	// bool isActive;
	// ... etc

private:
	ofxAssimpModelLoader model;
	ofVec3f position;
	bool scaleNormalization;
	ofMatrix4x4 modelMatrix;
};

