#include "BuildObstacles.h"
#include <cmath> // for std::nanf

Obstacle gap(float gapStartX, float gapLength, float height) {
    // Create a small horizontal segment at the same height as the ramp end
    std::vector<b2Vec2> vertices;
    vertices.push_back(b2Vec2(gapStartX, height));
    vertices.push_back(b2Vec2(gapStartX + gapLength, height));
    return { vertices, height, height, true }; // isGap = true
}



std::vector<Obstacle> createSimpleRampSet() {
    std::vector<Obstacle> obstacleSet;

    // First obstacle: line(10) + ramp(10)
    {
        std::vector<b2Vec2> vertices;
        vertices.push_back(b2Vec2(0.0f, 0.0f));
        vertices.push_back(b2Vec2(10.0f, 0.0f));
        vertices.push_back(b2Vec2(20.0f, 5.0f));
        Obstacle firstObstacle = { vertices, 0.0f, 5.0f, false };
        obstacleSet.push_back(firstObstacle);
    }

    // gap(5) at height=5 as a sensor line
    obstacleSet.push_back(gap(20.0f, 5.0f, 5.0f));

    // Second obstacle: line(10) at ground level (25,0)->(35,0)
    {
        std::vector<b2Vec2> vertices;
        vertices.push_back(b2Vec2(25.0f, 0.0f));
        vertices.push_back(b2Vec2(35.0f, 0.0f));
        Obstacle secondObstacle = { vertices, 5.0f, 0.0f, false };
        obstacleSet.push_back(secondObstacle);
    }

    return obstacleSet;
}
