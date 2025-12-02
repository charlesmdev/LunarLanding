//
//  RotationalPhysics3D.h
//  LunarLanding
//
//  Created by Charles M. on 12/2/25.
//
#pragma once
#include "Physics3D.h"
#include "Shape3D.h"

class RotationalPhysics3D : public Physics3D {
public:
	// rotation in degrees per second around x, y, z
	glm::vec3 angularVel   = glm::vec3(0, 0, 0);
	glm::vec3 angularAccel = glm::vec3(0, 0, 0);
	glm::vec3 torque       = glm::vec3(0, 0, 0); // simplistic: 1 inertia per axis
	float rotationalDamping = 0.99f;
	float angularScale = 5.0f;

	// override integrates both linear and angular motion
	void integrate(Shape3D& s, const glm::vec3& externalForce) override {
		float dt = ofGetLastFrameTime();

		// linear
		Physics3D::integrate(s, externalForce);

		// angular (very simplified: angularAccel = torque / mass)
		angularAccel = (torque / mass) * angularScale;
		s.rotation += angularVel * dt;
//		angularAccel = torque / mass;
		angularVel += angularAccel * dt;
		angularVel *= rotationalDamping;

		torque = glm::vec3(0, 0, 0);
	}

	void addTorque(const glm::vec3& t) {
		torque += t;
	}
};

