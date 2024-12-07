#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <vector>
#include <Box2D/Box2D.h>

struct Obstacle {
    std::vector<b2Vec2> vertices;
    float startHeight;
    float endHeight;
    bool isGap; // Make sure this is present and set accordingly
};

#endif // OBSTACLE_H
