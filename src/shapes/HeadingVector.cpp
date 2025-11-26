//
//  HeadingVector.cpp
//  mySketch
//
//  Created by Charles M. on 9/20/25.
//
#include "HeadingVector.h"
#include "Shape.h"

HeadingVector::HeadingVector(Shape& s) {
	shape = &s;
	lengthOffset = 0.0f;
}

void HeadingVector::draw() {
	yDirectionHeading = glm::vec3(0, shape->getHeight(), 0); // Put this dynamically instead since putting it in the constructor, the shape isn't initialized yet so the vector would just be (0, 0, 0)
	ofDrawLine(shape->getCenterPoint(), shape->getCenterPoint() - this->yDirectionHeading * 2);
	//	ofDrawLine(shape->getCenterPoint(), shape->getCenterPoint() - this->yDirectionHeading * 3); // *3 height, change direction heading to the height of the triangle
}

// Moves the heading along its direction (adjusts lengthOffset)
// void HeadingVector::moveAlongHeading(float movementSpeed) {
// 	glm::vec3 dir = glm::vec3(0, -1, 0); // pointing "up" in local space
// 	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(shape->rotation), glm::vec3(0, 0, 1));
// 	glm::vec3 worldDir = glm::vec3(rotMat * glm::vec4(dir, 0.0f));

// 	// Move shape's position along heading
// 	shape->position += worldDir * movementSpeed;
// }

glm::vec3 HeadingVector::getForward() {
    glm::vec3 localForward = glm::vec3(0, -1, 0); // local nose direction
    float rotRad = glm::radians(shape->getRotate());
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), rotRad, glm::vec3(0, 0, 1));
    glm::vec3 worldForward = glm::vec3(rotMat * glm::vec4(localForward, 0.0f));
    return worldForward;
}
