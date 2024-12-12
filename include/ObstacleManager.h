#pragma once

#include <vector>
#include "Obstacle.h"

// Manages a collection of obstacles.
class ObstacleManager {
public:
    void addObstacle(const Obstacle& obstacle);      // Adds an obstacle to the manager.
    const std::vector<Obstacle>& getObstacles() const; // Returns a constant reference to the vector of obstacles.
    void clearObstacles();                           // Clears all obstacles from the manager.

private:
    std::vector<Obstacle> obstacles; // Stores the collection of obstacles.
};