//
//  Lander.cpp
//  LunarLanding
//
//  Created by Charles M. on 12/2/25.
//
// Lander.cpp
#include "Lander.h"

Lander::Lander() {
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale    = glm::vec3(1, 1, 1);
}

bool Lander::loadModel(const std::string &path) {
	bool loaded = model.loadModel(path);
	return loaded;
}

void Lander::applyTransformToModel() {
	// position
	model.setPosition(position.x, position.y, position.z);

	// rotation: oF Assimp loader has separate rotations by axis index
	// Example: index 0,1,2 for x,y,z; adapt to your existing usage.
	model.setRotation(0, rotation.x, 1, 0, 0);
	model.setRotation(1, rotation.y, 0, 1, 0);
	model.setRotation(2, rotation.z, 0, 0, 1);

	// scale
	model.setScale(scale.x, scale.y, scale.z);
}

void Lander::drawWireframe() {
	applyTransformToModel();
	model.drawWireframe();
}

void Lander::drawFaces() {
	applyTransformToModel();
	model.drawFaces();
}

void Lander::setPosition(float x, float y, float z) {
	Shape3D::setPosition(x, y, z);
	// keep model in sync
	model.setPosition(x, y, z);
}

glm::vec3 Lander::getPosition() const {
	return position;
}

void Lander::updatePhysics(const glm::vec3& externalForce) {
	physics.integrate(*this, externalForce);
}

void Lander::setScaleNormalization(bool normalize) {
	scaleNormalization = normalize;
	model.setScaleNormalization(normalize);
}

int Lander::getNumMeshes() {
	return model.getNumMeshes();
}

ofMesh Lander::getMesh(int i) {
	return model.getMesh(i);
}

int Lander::getMeshCount() {
	return model.getMeshCount();
}

glm::vec3 Lander::getSceneMin() {
	return model.getSceneMin();
}

glm::vec3 Lander::getSceneMax() {
	return model.getSceneMax();
}

ofMatrix4x4 Lander::getModelMatrix() {
	return model.getModelMatrix();
}



