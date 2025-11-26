//
//  Testing.h
//  mySketch
//
//  Created by Charles M. on 9/20/25.
//
#include "ofMain.h"

class Testing {
public:
	glm::vec3 testVector = glm::vec3(1, 0, 1);
	Testing() {

	}
	void draw() {
		ofDrawCircle(testVector, 5);
	}
};
