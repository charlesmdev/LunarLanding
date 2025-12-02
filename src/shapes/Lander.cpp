//
//  Lander.cpp
//  LunarLanding
//
//  Created by Charles M. on 12/2/25.
//
#include "Lander.h"

Lander::Lander()
	: position(0, 0, 0), scaleNormalization(false)
{
}

bool Lander::loadModel(const std::string &path) {
	bool loaded = model.loadModel(path);
	return loaded;
}

void Lander::drawWireframe() {
	model.setPosition(position.x, position.y, position.z);
	model.drawWireframe();
}

void Lander::drawFaces() {
	model.setPosition(position.x, position.y, position.z);
	model.drawFaces();
}

void Lander::setPosition(float x, float y, float z) {
	position.set(x, y, z);
	model.setPosition(x, y, z);
}

ofVec3f Lander::getPosition() const {
	return position;
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

ofVec3f Lander::getSceneMin() {
	return model.getSceneMin();
}

ofVec3f Lander::getSceneMax() {
	return model.getSceneMax();
}

ofMatrix4x4 Lander::getModelMatrix() {
	return model.getModelMatrix();
}


