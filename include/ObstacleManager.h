#ifndef OBSTACLEMANAGER_H
#define OBSTACLEMANAGER_H

#include "Obstacle.h"
#include <vector>

class ObstacleManager {
public:
    void addObstacle(const Obstacle& obstacle);
    void generateTrack(std::vector<b2Vec2>& trackVertices, int numObstacles);
    const std::vector<Obstacle>& getObstacles() const;

private:
    std::vector<Obstacle> obstacles;
};

#endif // OBSTACLEMANAGER_H
