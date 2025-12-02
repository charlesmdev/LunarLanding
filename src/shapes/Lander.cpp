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

//glm::vec3 Lander::getWorldThrustDir() const {
//	// Use this->rotation (Euler degrees) to rotate local up (0,1,0)
//	glm::vec3 rotRad = glm::radians(rotation);
//
//	glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), rotRad.x, glm::vec3(1, 0, 0));
//	glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), rotRad.y, glm::vec3(0, 1, 0));
//	glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), rotRad.z, glm::vec3(0, 0, 1));
//
//	glm::mat4 R = Rz * Ry * Rx;
//
//	glm::vec4 localUp(0, 1, 0, 0); // thrust along local +Y
//	glm::vec4 worldUp = R * localUp;
//
//	return glm::normalize(glm::vec3(worldUp));
//}

glm::vec3 Lander::getForwardDir() const {
	// Heading is rotation.y in degrees; forward = -Z in local space
	float yawRad = glm::radians(rotation.y);
	// Standard FPS-style forward in XZ plane:
	// forward = (sin(yaw), 0, -cos(yaw))
	glm::vec3 f(std::sin(yawRad), 0.0f, -std::cos(yawRad));
	return glm::normalize(f);
}

glm::vec3 Lander::getRightDir() const {
	// Right is +90 degrees from forward in XZ plane
	float yawRad = glm::radians(rotation.y + 90.0f);
	glm::vec3 r(std::sin(yawRad), 0.0f, -std::cos(yawRad));
	return glm::normalize(r);
}

glm::vec3 Lander::getUpDir() const {
	// Up is just world up if you only want heading-based movement
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



