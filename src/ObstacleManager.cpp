#include "ObstacleManager.h"

void ObstacleManager::addObstacle(const Obstacle& obstacle) {
    obstacles.push_back(obstacle);
}

const std::vector<Obstacle>& ObstacleManager::getObstacles() const {
    return obstacles;
}
