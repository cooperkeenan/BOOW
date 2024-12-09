#include "ObstacleManager.h"
#include <iostream>

void ObstacleManager::addObstacle(const Obstacle& obstacle) {
    obstacles.push_back(obstacle);
}

const std::vector<Obstacle>& ObstacleManager::getObstacles() const {
    return obstacles;
}

std::vector<Obstacle> ObstacleManager::buildLevel(const std::vector<Obstacle>& rawObstacles) {
    // If the obstacles are already in their final position, just return them as is.
    return rawObstacles;
}
