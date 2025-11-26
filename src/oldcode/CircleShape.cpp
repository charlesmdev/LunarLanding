//
//  CircleShape.cpp
//  mySketch
//
//  Created by Charles M. on 9/8/25.
//
#include "CircleShape.h"

//void CircleShape::draw() {
//	ofDrawCircle(this->position, this->radius);
//}
//
bool CircleShape::inside(glm::vec3 p) {
//	if(distance(this->position, p) < this->radius)
//		return true;
//	else
//		return false;
	glm::vec3 p1 = glm::inverse(getTransform()) * glm::vec4(p, 1); // Why glm::vec4? This is bacause the matrix is 4x4, multiplying by it needs a column that matches that dimension. Computer graphics, converting points from one coordinate frame or space to another.
//	return (glm::distance(p, position)) < radius;
	return (glm::distance(p1, glm::vec3(0, 0, 0))) < radius; // We want to compare it to the orignal position of the shape so (0, 0, 0)
}


