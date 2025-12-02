//
//  Physics3D.h
//  LunarLanding
//
//  Created by Charles M. on 12/2/25.
//
#pragma once
#include "ofMain.h"

class Shape3D;

class Physics3D {
public:
	glm::vec3 vel   = glm::vec3(0, 0, 0);
	glm::vec3 accel = glm::vec3(0, 0, 0);
	glm::vec3 force = glm::vec3(0, 0, 0);

	float damping = 0.99f;
	float mass    = 0.8f;

	Physics3D() {}

	virtual void integrate(Shape3D& s, const glm::vec3& externalForce) {
		if (glm::any(glm::isnan(vel))) {
			vel = glm::vec3(0, 0, 0);
		}

		float dt = ofGetLastFrameTime(); // seconds[web:18]

		// accumulate forces
		force += externalForce;

		// integrate
		s.position += vel * dt;
		accel = (1.0f / mass) * force;
		vel += accel * dt;
		vel *= damping;

		// clear for next frame
		force = glm::vec3(0, 0, 0);
	}

	void addForce(const glm::vec3& f) {
		force += f;
	}
};
