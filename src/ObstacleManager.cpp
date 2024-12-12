#include "ObstacleManager.h"

// Adds an obstacle to the collection of obstacles.
void ObstacleManager::addObstacle(const Obstacle& obstacle) {
    obstacles.push_back(obstacle); // Add the given obstacle to the vector of obstacles.
}

// Returns a constant reference to the vector of obstacles.
const std::vector<Obstacle>& ObstacleManager::getObstacles() const {
    return obstacles; // Return a constant reference to prevent modification of the original vector.
}

// Clears all obstacles from the collection.
void ObstacleManager::clearObstacles() {
    obstacles.clear(); // Remove all elements from the obstacles vector.
}