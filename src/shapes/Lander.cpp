//
//  Lander.cpp
//  LunarLanding
//
//  Created by Charles M. on 12/2/25.
//
// Lander.cpp
#include "Lander.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const glm::vec3 HEADING = glm::vec3(0, 0, 1); // Heading Vector

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
	model.setPosition(position.x, position.y, position.z);
	model.setRotation(0, rotation.y, 0, 1, 0);
	model.setScale(scale.x, scale.y, scale.z);
}

glm::vec3 Lander::getForwardDir() const {
	float yawRad = glm::radians(rotation.y);
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), yawRad, glm::vec3(0, 1, 0));
	glm::vec3 worldForward = glm::vec3(rotMat * glm::vec4(HEADING, 0.0f));
	return glm::normalize(worldForward);
}

glm::vec3 Lander::getRightDir() const {
	glm::vec3 fwd = getForwardDir();
	glm::vec3 up  = getUpDir();
	return glm::normalize(glm::cross(fwd, up)); 
}

glm::vec3 Lander::getUpDir() const {
	return glm::vec3(0, 1, 0);
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



