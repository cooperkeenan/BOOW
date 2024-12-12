#pragma once

#include <vector>
#include <Box2D/Box2D.h>

// Represents an obstacle in the game world.
struct Obstacle {
    std::vector<b2Vec2> vertices; // Vertices defining the shape of the obstacle.
    float startHeight;            // Starting height of the obstacle.
    float endHeight;              // Ending height of the obstacle.
    bool isGap;                   // Flag indicating if the obstacle is a gap.
};