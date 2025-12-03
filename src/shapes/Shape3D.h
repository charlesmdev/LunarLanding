//
//  Shape3D.h
//  LunarLanding
//
//  Created by Charles M. on 12/2/25.
//
#pragma once
#include "ofMain.h"

class Shape3D {
public:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0); 
	glm::vec3 scale    = glm::vec3(1, 1, 1);

	virtual ~Shape3D() {}

	virtual void setPosition(const glm::vec3& p) {
		position = p;
	}

	virtual void setPosition(float x, float y, float z) {
		position = glm::vec3(x, y, z);
	}

	virtual glm::vec3 getPosition() const {
		return position;
	}

	virtual void setRotation(const glm::vec3& rDeg) {
		rotation = rDeg;
	}

	virtual glm::vec3 getRotation() const {
		return rotation;
	}

	virtual void setScale(const glm::vec3& s) {
		scale = s;
	}

	virtual glm::vec3 getScale() const {
		return scale;
	}
};
