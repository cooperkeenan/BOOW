#pragma once

#include <vector>
#include <Box2D/Box2D.h>

struct Obstacle {
    std::vector<b2Vec2> vertices;
    float startHeight;
    float endHeight;
    bool isGap; 
};