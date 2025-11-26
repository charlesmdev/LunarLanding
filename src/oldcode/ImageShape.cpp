//
//  ImageShape.cpp
//  mySketch
//
//  Created by Charles M. on 9/20/25.
//
#include "ImageShape.h"

void ImageShape::draw() {

	ofPushMatrix(); // Store state of current transformation matrix
	ofMultMatrix(getTransform());
//	this->myImage.draw(position.x, position.y);
	myImage.draw(-myImage.getWidth() / 2.0f,
				 -myImage.getHeight() / 2.0f);
	ofPopMatrix();
}
glm::vec3 ImageShape::getCenterPoint() {
//	return glm::vec3(-myImage.getWidth() / 2.0f, -myImage.getHeight() / 2.0f, 0);
	return glm::vec3(0, 0, 0);  // because we now draw centered
}
float ImageShape::getHeight() {
	return myImage.getHeight();
}

bool ImageShape::inside(glm::vec3 p) {
	glm::vec3 localP = glm::vec3(glm::inverse(getTransform()) * glm::vec4(p, 1));
	float halfW = myImage.getWidth() / 2.0f;
	float halfH = myImage.getHeight() / 2.0f;
	return (localP.x >= -halfW && localP.x <= halfW && localP.y >= -halfH && localP.y <= halfH);
}
