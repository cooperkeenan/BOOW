#ifndef OBSTACLEMANAGER_H
#define OBSTACLEMANAGER_H

#include "Obstacle.h"
#include <vector>

class ObstacleManager {
public:
    void addObstacle(const Obstacle& obstacle);
    const std::vector<Obstacle>& getObstacles() const;

private:
    std::vector<Obstacle> obstacles;
};

#endif // OBSTACLEMANAGER_H
