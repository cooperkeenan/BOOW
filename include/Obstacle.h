//Obstacle.h


#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <vector>
#include <Box2D/Box2D.h>

struct Obstacle {
    std::vector<b2Vec2> vertices; // Vertices of the obstacle
    float startHeight;            // Starting height
    float endHeight;              // Ending height
};

#endif // OBSTACLE_H
