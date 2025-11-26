//
//  CircleShape.h
//  mySketch
//
//  Created by Charles M. on 9/8/25.
//
#pragma once
#include "Shape.h"
class CircleShape : public Shape {
public:
	int radius;
//	void draw() override { ofDrawCircle(this->position, this->radius); }
	void draw() override {
		ofPushMatrix(); // Store state of current transformation matrix
		ofMultMatrix(getTransform()); // Multiply current transformation matrix by M
		//
		ofSetCircleResolution(500); // To smooth a circle with 100 line segments. Since curved objects or circles are made from line segments
		ofDrawCircle(glm::vec3(0, 0, 0), radius); // Draw the circle
		ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(radius, 0, 0));
		//
		ofPopMatrix(); // Restore state of current transformation matrix
	}

	bool inside(glm::vec3 p) override;
	
	CircleShape(int r) {
		this->radius = r;
	}
	
	CircleShape(glm::vec3 p, int r) {
		this->radius = r;
		this->position = p;
	}
};


// header files keep one liner functions in it. If a function is more complex create a separate cpp file
