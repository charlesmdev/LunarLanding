//
//  HeadingVector.h
//  mySketch
//
//  Created by Charles M. on 9/20/25.
//
#pragma once
#include "ofMain.h"


class Shape;

class HeadingVector {
public:
	
	
	int multiplier;
	float rotation = 0.0f;
	bool toggle = true;
	Shape* shape;  // pointer instead of reference;
	HeadingVector(Shape& s);
	glm::vec3 yDirectionHeading;
	glm::vec3 xDirectionHeading;
	
	void draw();
	void length();
	// void moveAlongHeading(float movementSpeed);
	float lengthOffset = 0.0f; // movement along the heading vector
	glm::vec3 getForward();
};
