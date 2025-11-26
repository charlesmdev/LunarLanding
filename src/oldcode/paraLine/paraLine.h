//
//  paraLine.cpp
//  mySketch
//
//  Created by Charles M. on 8/29/25.
//
#include "ofMain.h"
#include "ofApp.h"
#include <vector>

class paraLine {
public:
	ofPolyline line;
	glm::vec3 p1;
	glm::vec3 p2;
	
	std::vector<glm::vec3> t_points;
	
	paraLine(glm::vec3 point1, glm::vec3 point2) {
		p1 = point1;
		p2 = point2;
	}
	
	void drawParaLine();
	
	bool isCursorClose(glm::vec3 point);
	
	void addPoint(glm::vec3 point);
	

};

