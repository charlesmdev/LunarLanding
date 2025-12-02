//
//  Lander.h
//  LunarLanding
//
//  Created by Charles M. on 12/2/25.
//
// Lander.h
#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Box.h"
#include "Shape3D.h"
#include "physics/RotationalPhysics3D.h"

class Lander : public Shape3D {
public:
	Lander();

	bool loadModel(const std::string &path);
	void drawWireframe();
	void drawFaces();

	void setPosition(float x, float y, float z) override;
	glm::vec3 getPosition() const override;

	void updatePhysics(const glm::vec3& externalForce = glm::vec3(0, 0, 0));

	void setScaleNormalization(bool normalize);
	int getNumMeshes();
	ofMesh getMesh(int i);
	int getMeshCount();
	glm::vec3 getSceneMin();
	glm::vec3 getSceneMax();
	ofMatrix4x4 getModelMatrix();

	RotationalPhysics3D physics;

	glm::vec3 thrustDir   = glm::vec3(0, 1, 0);  // direction of thrust (up by default)
	float     thrustAmount = 0.0f;              // current thrust magnitude from GUI
	float     thrustMax    = 20.0f;             // max thrust (for clamping / tuning)
	glm::vec3 torqueScale  = glm::vec3(0, 0, 10.0f); // optional if you want scaled torque
	
	glm::vec3 getWorldThrustDir() const;

private:
	ofxAssimpModelLoader model;
	bool scaleNormalization = false;

	void applyTransformToModel(); // helper
};


