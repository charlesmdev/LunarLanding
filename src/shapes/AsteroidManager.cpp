//#include "AsteroidManager.h"
//
//using namespace std;
//
//AsteroidManager::AsteroidManager() : collisionManager(nullptr) {
//}
//
//AsteroidManager::~AsteroidManager() {
//    clear();
//}
//
//void AsteroidManager::setup(int numAsteroids, int min, int numSplits, CollisionManager* colManager) {
//    clear();
//    splitCount = numSplits;
//    collisionManager = colManager;
//    scoreCount = 0;
//
//    // Used for determinin how many asteroids should be on screen. With a minimum and maximum number. Currently max is 2 while a minimum is 1. Meaning, 2 big asteroids should be on screen.
//    this->maxAsteroids = numAsteroids;
//    this->minAsteroids = min;
//
//    spawnWave(numAsteroids);
//}
//
//void AsteroidManager::spawnWave(int numToSpawn) {
//    for (int i = 0; i < numToSpawn; i++) {
//        Asteroid* a = new Asteroid(splitCount);
//        asteroids.push_back(a);
//        
//        if (collisionManager) {
//            collisionManager->addCollider(&a->col);
//        }
//    }
//}
//
//void AsteroidManager::update() {
//    for (auto* a : asteroids) {
//        a->move(glm::vec3(0, 0, 0));
//        a->rotate(0);
//        if(showAsteroidsBoundShapes) a->showBoundingCircle = showAsteroidsBoundShapes;
//    }
//
//    // This complicated reference pointer hell of code removes asteroids from the list after they die. To free up memory.
//    for (auto a = asteroids.begin(); a != asteroids.end(); ) {
//    if (!((*a)->visible)) {
//        if (collisionManager) {
//            collisionManager->removeCollider(&(*a)->col);
//        }
//        delete *a;  // Free memory
//        a = asteroids.erase(a);  
//    } else {
//        ++a;
//    }
//}
//
//    int parentCount = getParentCount();
//    if (parentCount < minAsteroids) {
//        int toSpawn = maxAsteroids - parentCount;  // Spawn up to max
//        spawnWave(toSpawn);
//    }
//}
//
//void AsteroidManager::draw() {
//    for (auto* a : asteroids) {
//        a->draw();
//    }
//}
//
//void AsteroidManager::removeAsteroid(Collision* col) {
//    for (auto* a : asteroids) {
//        if (&a->col == col) {
//            vector<Asteroid*> children = a->split();
//            for (auto* child : children) {
//                asteroids.push_back(child);
//                if (collisionManager) {
//                    collisionManager->addCollider(&child->col);
//                }
//            }
//            a->die();
//            scoreCount++;
//            break;
//        }
//    }
//}
//
//void AsteroidManager::clear() {
//    // Delete all asteroids and free memory
//    for (auto* a : asteroids) {
//        if (collisionManager) {
//            collisionManager->removeCollider(&a->col);
//        }
//        delete a;
//    }
//    asteroids.clear();
//}
//
//int AsteroidManager::getParentCount() const {
//    int count = 0;
//    for (auto* a : asteroids) {
//        if (a->generation == 0) {  // Only count parent asteroids
//            count++;
//        }
//    }
//    return count;
//}
