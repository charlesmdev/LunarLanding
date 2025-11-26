// This class has helper functions for some of the game mechanics, such as entities wrapping round the screen and checking distance between objects.
#pragma once
#include "ofMain.h"

// You can add more helpers later, like random vectors, distances, etc.
namespace Helpers {

    // Wrap a position around the screen boundaries, method needed for both Asteroid.h and Player.h
    inline void wrapAround(glm::vec3 &pos) {
        float width = ofGetWidth();
        float height = ofGetHeight();

        if (pos.x > width)  pos.x = 0;
        else if (pos.x < 0) pos.x = width;

        if (pos.y > height) pos.y = 0;
        else if (pos.y < 0) pos.y = height;
    }

    // Return a random spawn position on any screen edge. Honestly this could be combined with Asteroid.cpp no idea I decided to make it a helper function.
    inline glm::vec3 randomSpawn() {
        float w = ofGetWidth();
        float h = ofGetHeight();
        int edge = (int)ofRandom(0, 4); 
        switch(edge) {
            case 0: return glm::vec3(ofRandom(0, w), 0, 0); // top
            case 1: return glm::vec3(ofRandom(0, w), h, 0); // bottom
            case 2: return glm::vec3(0, ofRandom(0, h), 0); // left
            case 3: return glm::vec3(w, ofRandom(0, h), 0); // right
            default: return glm::vec3(w/2, h/2, 0); // fallback center
        }
    }
    


}