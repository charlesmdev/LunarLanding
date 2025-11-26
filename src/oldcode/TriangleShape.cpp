//
//  TriangleShape.cpp
//  mySketch
//
//  Created by Charles M. on 9/8/25.
//
#include "TriangleShape.h"
#include "HeadingVector.h"

TriangleShape::TriangleShape() {
	// Default will draw an equilateral triangle at the center of the screen

	// 3 vertices at 120° apart
	for (int i = 0; i < 3; i++) {
		float angle = glm::radians(120.0f * i - 90.0f); // start at top
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		vertices.push_back(glm::vec3(x, y, 0.0f));
	}
}

void TriangleShape::draw() {
	ofPushMatrix(); // Store state of current transformation matrix
	ofMultMatrix(getTransform());
	this->polyShape.clear();
	
	for(auto& vertice : vertices) {
		this->polyShape.addVertex(vertice); // Use ofPolyline instead so we dont have to transform 3 different line matrices
	}
	this->polyShape.close();
	this->polyShape.draw();

	
	ofPopMatrix();
	
//	ofDrawLine(vertices[0], vertices[1]);
//	ofDrawLine(vertices[1], vertices[2]);
//	ofDrawLine(vertices[2], vertices[0]);
}

glm::vec3 TriangleShape::getCenterPoint() {
	// To compute the centroid of the triangle, we would add all the vertices then divide by 3.
	float xCentroid = 0.0;
	float yCentroid = 0.0;
	for(auto& vertice : vertices) {
		xCentroid += vertice.x;
		yCentroid += vertice.y;
	}
	return glm::vec3(xCentroid / 3, yCentroid /3, 0.0f);
//	return this->position; // Temporary
}

float TriangleShape::getHeight() {
	if (vertices.size() < 2)
		return 0.0f;
	float sideLength = glm::distance(vertices[0], vertices[1]);
	return (sqrt(3.0f) / 2.0f) * sideLength;
}

// Uses Barycentric coordinates??? No clue how this math works lol
bool TriangleShape::inside(glm::vec3 p) {
	glm::vec3 localP = glm::vec3(glm::inverse(getTransform()) * glm::vec4(p, 1));

	
	float d1, d2, d3;
	bool has_neg, has_pos;
	
	d1 = sign(localP, vertices[0], vertices[1]);
	d2 = sign(localP, vertices[1], vertices[2]);
	d3 = sign(localP, vertices[2], vertices[0]);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
	
//	return false;
}

float TriangleShape::sign(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

// void TriangleShape::move(glm::vec3 p) {
// 	this->position = p;
// }

void TriangleShape::rotateBy(float r) {
	this->rotation += r;
}

// For rotating triangle towards desired point
void TriangleShape::rotateToward(glm::vec3 target, float step) {
	targetPoint = target;
	rotationStep = step;
	rotatingToPoint = true;

	// Compute target angle once
	glm::vec2 dir = glm::vec2(targetPoint - (position + getCenterPoint()));
	targetAngle = glm::degrees(atan2(dir.y, dir.x)) + 90.0f; // -90 aligns with "up"
}

void TriangleShape::updateRotation() {
	if (!rotatingToPoint) return;

	float diff = targetAngle - rotation;
	diff = fmod(diff + 180.0f, 360.0f) - 180.0f; // Wrap into [-180,180]

	if (fabs(diff) < rotationStep) {
		rotation = targetAngle;
		rotatingToPoint = false; // Done
	} else {
		rotation += (diff > 0 ? rotationStep : -rotationStep);
	}
}
