#pragma once
#include <vector>
#include "Obstacle.h"

class ObstacleManager {
public:
    void addObstacle(const Obstacle& obstacle);
    const std::vector<Obstacle>& getObstacles() const;
    void clearObstacles(); 


private:
    std::vector<Obstacle> obstacles;
};

