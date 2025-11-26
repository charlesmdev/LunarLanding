//
//  TriangleShape.h
//  mySketch
//
//  Created by Charles M. on 9/8/25.
//
#pragma once
#include "Shape.h"
class TriangleShape : public Shape {
public:
	float radius = 100.0f;
	ofPolyline polyShape;
	TriangleShape();
	// Constructors involving position, or not, and vertices.
	TriangleShape(vector<glm::vec3> v) {
		this->vertices = v;
	}
	TriangleShape(glm::vec3 p, vector<glm::vec3> v) {
		this->position = p;
		this->vertices = v;
	}
	TriangleShape(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
		this->vertices.push_back(a);
		this->vertices.push_back(b);
		this->vertices.push_back(c);
	}
	TriangleShape(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
		this->position = p;
		this->vertices.push_back(a);
		this->vertices.push_back(b);
		this->vertices.push_back(c);
	}
	//
	void draw() override;
	glm::vec3 getCenterPoint() override;
	float getHeight() override;
	bool inside(glm::vec3 p) override;
	float sign(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	//void move(glm::vec3 p) override;
	void rotateBy(float r) override;
	
	// For rotating to circle point
	void rotateToward(glm::vec3 target, float step = 2.0f);
	void updateRotation();
	bool rotatingToPoint = false;
	glm::vec3 targetPoint;
	float rotationStep = 2.0f;
	float targetAngle = 0.0f;
};
