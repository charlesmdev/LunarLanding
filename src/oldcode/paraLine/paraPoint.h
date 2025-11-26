//
//  paraPoint.h
//  mySketch
//
//  Created by Charles M. on 8/29/25.
//

class paraPoint() {
public:
	std::vec3 p;
	paraPoint(std::vec3 point) {
		p = point;
	}
	
	void slidePos();
	bool isHover();
	void drawPoint();
	float displayTValue();
};
