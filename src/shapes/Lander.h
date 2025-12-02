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

private:
	ofxAssimpModelLoader model;
	bool scaleNormalization = false;

	void applyTransformToModel(); // helper
};


