//
//  LandingZone.h
//  LunarLanding
//
//  Created by Charles M. on 12/8/25.
//
#pragma once
#include "ofMain.h"
#include "box.h"     // same Box/Vector3 you use with the octree

class LandingZone {
public:
	LandingZone() {}
	LandingZone(const glm::vec3 &center, const glm::vec3 &halfSize)
	{
		set(center, halfSize);
	}

	void set(const glm::vec3 &center, const glm::vec3 &halfSize) {
		glm::vec3 minPt = center - halfSize;
		glm::vec3 maxPt = center + halfSize;
		box = Box(Vector3(minPt.x, minPt.y, minPt.z),
				  Vector3(maxPt.x, maxPt.y, maxPt.z));
	}

	// Simple AABB overlap test with another Box
	bool overlaps(const Box &other) const {
		return box.overlap(other);
	}

	// Optional debug draw
	void draw() const {
		ofNoFill();
		ofSetColor(ofColor::yellow);
		Octree::drawBox(box);
	}

	Box box;
};

