//
//  Shape.cpp
//  mySketch
//
//  Created by Charles M. on 9/8/25.
//
#include "Shape.h"
#include "HeadingVector.h"
glm::mat4 Shape::getTransform() {
	glm::mat4 T = glm::translate(glm::mat4(1.0), this->position); // Translation matrix from the identity matrix
	glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(this->rotation), glm::vec3(0, 0, 1)); // 2d we're rotating around a point. We use mat4 instead of mat3 because we are translating 2d objects in a 3d space
	glm::mat4 S = glm::scale(glm::mat4(1.0), this->scale);
	return (T * R * S); // Results depend on this order. Generally Scale first, Rotate, and Translate. For GLM, the order is from right to left so this is alright. Row major column major?
}

void Shape::draw() {
	ofPushMatrix(); // Store state of current transformation matrix
	ofMultMatrix(getTransform());
	
	cout << "Drawing circle at " << this->position << endl;
	ofDrawCircle(this->position, 5);
}

Shape::Shape() {
	heading = new HeadingVector(*this); 
}

glm::vec3 Shape::getCenterPoint() {
	return glm::vec3(0, 0 , 0);
}

void Shape::checkBounds() {
    float screenW = ofGetWidth();
    float screenH = ofGetHeight();

    float halfW = 20 * scale.x; 
    float halfH = 20 * scale.y; 

    if(position.x - halfW < 0) position.x = halfW;
    if(position.x + halfW > screenW) position.x = screenW - halfW;

    if(position.y - halfH < 0) position.y = halfH;
    if(position.y + halfH > screenH) position.y = screenH - halfH;
}

