#include "ObstacleManager.h"

// Add a new obstacle to the manager's list.
void ObstacleManager::addObstacle(const Obstacle& obstacle) {
    obstacles.push_back(obstacle);
}

// Retrieve a constant reference to the current list of obstacles.
const std::vector<Obstacle>& ObstacleManager::getObstacles() const {
    return obstacles;
}

// Clear all obstacles from the manager, typically when resetting or loading a new level.
void ObstacleManager::clearObstacles() {
    obstacles.clear();
}
