#pragma once
#include "ofMain.h"
#include "shapes/Asteroid.h"
#include "physics/CollisionManager.h"
#include <vector>

using namespace std;

class AsteroidManager {
public:
    AsteroidManager();
    ~AsteroidManager();
    bool showAsteroidsBoundShapes = false;
    
    void setup(int numAsteroids, int minAsteroids, int numSplits, CollisionManager* colManager); // add Num of splits here
    void update();
    void draw();
    void clear();
    
    void removeAsteroid(Collision* col);
    int getCount() const { return asteroids.size(); }
    int getParentCount() const;
    void spawnWave(int numToSpawn);  // Add this
    int getScore() const { return scoreCount; }
    
    vector<Asteroid*> asteroids;

    void setSplitCount(int count) { splitCount = count; }
    int getSplitCount() const { return splitCount; }
    
private:
    CollisionManager* collisionManager;    
    int splitCount = 0;
    int minAsteroids = 0;
    int maxAsteroids = 0; 
    int scoreCount = 0;
};