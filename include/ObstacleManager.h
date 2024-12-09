#pragma once
#include <vector>
#include "Obstacle.h"

class ObstacleManager {
public:
    void addObstacle(const Obstacle& obstacle);
    const std::vector<Obstacle>& getObstacles() const;

    // This function takes in a raw list of obstacle prototypes (all starting at (0,0)) 
    // and places them end-to-end.
    std::vector<Obstacle> buildLevel(const std::vector<Obstacle>& rawObstacles);

private:
    std::vector<Obstacle> obstacles;
};
