#include "BuildObstacles.h"

Obstacle gap(float gapLength) {
    std::vector<b2Vec2> gapVertices;
    // One vertex to represent gap length
    gapVertices.push_back(b2Vec2(gapLength, 0.0f));
    return { gapVertices, 0.0f, 0.0f };
}

std::vector<Obstacle> createSimpleRampSet() {
    std::vector<Obstacle> obstacleSet;

    // First obstacle: line(10) + ramp(10)
    {
        std::vector<b2Vec2> vertices;
        // line(10): (0,0) to (10,0)
        vertices.push_back(b2Vec2(0.0f, 0.0f));
        vertices.push_back(b2Vec2(10.0f, 0.0f));
        // ramp(10): (10,0) to (20,5)
        vertices.push_back(b2Vec2(20.0f, 5.0f));

        // startHeight = 0, endHeight = 5
        Obstacle firstObstacle = { vertices, 0.0f, 5.0f };
        obstacleSet.push_back(firstObstacle);
    }

    // gap(5)
    obstacleSet.push_back(gap(5.0f));

    // Second obstacle: line(10) at ground level
    {
        std::vector<b2Vec2> vertices;
        // (25,0) to (35,0)
        vertices.push_back(b2Vec2(25.0f, 0.0f));
        vertices.push_back(b2Vec2(35.0f, 0.0f));

        // startHeight=5.0f to match the previous currentHeight 
        // ensuring no vertical offset in ObstacleManager logic
        Obstacle secondObstacle = { vertices, 5.0f, 0.0f };
        obstacleSet.push_back(secondObstacle);
    }

    return obstacleSet;
}
