#include "ObstacleManager.h"

void ObstacleManager::addObstacle(const Obstacle& obstacle) {
    obstacles.push_back(obstacle);
}

const std::vector<Obstacle>& ObstacleManager::getObstacles() const {
    return obstacles;
}

std::vector<Obstacle> ObstacleManager::buildLevel(const std::vector<Obstacle>& rawObstacles) {
    // Return as is since obstacles are already placed in global coordinates
    return rawObstacles;
}
