//
//  paraLine.cpp
//  mySketch
//
//  Created by Charles M. on 8/29/25.
//
#include "paraLine.h"
#include "ofApp.h"

void paraLine::drawParaLine() {
	line.addVertex(p1.x, p1.y);
	line.addVertex(p2.x, p2.y);
	line.draw();
}

bool paraLine::isCursorClose(glm::vec3 point) {
	// Check if cursor is close to line
	return true;
}

void paraLine::addPoint(glm::vec3 point) {
	
}
