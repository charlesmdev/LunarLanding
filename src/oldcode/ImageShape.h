//
//  ImageShape.h
//  mySketch
//
//  Created by Charles M. on 9/20/25.
//
#pragma once
#include "ofMain.h"
#include "TriangleShape.h"
#include "HeadingVector.h"

class ImageShape : public TriangleShape {
public:
	ofImage myImage;
	ImageShape() {
		myImage.load("images/fighterjet.png");
	}
	void draw() override;
	glm::vec3 getCenterPoint() override;
	float getHeight() override;
	bool inside(glm::vec3 p) override;
	
	// Maybe add in the future to load other images
//	void loadImage(){
//		
//	}
//	ImageShape(char[] imagePath) {
//		
//	}
};
