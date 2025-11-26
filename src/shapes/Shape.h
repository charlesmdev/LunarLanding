//
//  Shape.h
//  mySketch
//
//  Created by Charles M. on 9/8/25.
//
#pragma once // Important
#include "ofMain.h"
#include <vector>

class HeadingVector; // forward declaration

class Shape {
public:
	HeadingVector* heading; // use pointer instead of value
	glm::vec3 position = glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0.0f); // Default Position
	bool toggleHeading = true;
	glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);
	float rotation = 0.0f;
	// Color color;
	vector<glm::vec3> vertices; // For other shapes with vertices

	Shape();
	Shape(vector<glm::vec3> v) {
		this->vertices = v;
	}
	glm::mat4 getTransform();
	virtual void draw(); 
	virtual void move(float movementSpeed) {
		// cout << "Moving using shape base" << endl;	
		glm::vec3 dir = glm::vec3(0, -1, 0); // pointing "up" in local space
		glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation), glm::vec3(0, 0, 1));
		glm::vec3 worldDir = glm::vec3(rotMat * glm::vec4(dir, 0.0f));
		position += worldDir * movementSpeed;
	}
	virtual void move(glm::vec3 force){}
	virtual void rotate(float torque){}
	//
	virtual glm::vec3 getCenterPoint(); // Gets center point of shape.
	virtual float getHeight(){ return 0.0f; }
	virtual void setHeadingVector() {} // Sets heading vector of shape.
	virtual void setVertices(vector<glm::vec3> v) {
		this->vertices = v;
	}
	virtual void rotateBy(float r) { this->rotation += r; }
	
	virtual void updateRotation() {}
	virtual void rotateToward(glm::vec3 p, float) {}
	void toggleHeadingVector() {
		if(toggleHeading == true)
			toggleHeading = false;
		else
			toggleHeading = true;
	}
	//
	virtual bool inside(glm::vec3 p) { return false; }
//	void setColor(Color c) {
//		this->color = c;
//	}
	void setSize(float s) {
		this->scale = glm::vec3(s, s, s);
	}
	void setPosition(glm::vec3 p) {
		this->position = p;
	}
	glm::vec3 getPosition() {
		return this->position;
	}
	void setRotate (float rot) {
		this->rotation = rot;
	}
	float getRotate() {
		return rotation;
	}


	void checkBounds();
//	~Shape() {} Destructor use if you are allocating mem
	
};

