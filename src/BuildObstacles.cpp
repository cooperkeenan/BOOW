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

    // Define start coordinates for the first obstacle
    float initialStartX = -20.0f;  // Hard coded start x value
    float initialStartY = 20.0f;  // Hard coded start y value

    // First obstacle: line(10) + ramp(10)
    {
        std::vector<b2Vec2> vertices;
        vertices.push_back(b2Vec2(initialStartX, initialStartY)); // Starting at (5,0)
        vertices.push_back(b2Vec2(initialStartX + 10.0f, initialStartY)); // Continues horizontally to (15,0)
        vertices.push_back(b2Vec2(initialStartX + 20.0f, initialStartY + 5.0f)); // Ramps up to (25,5)
        Obstacle firstObstacle = { vertices, initialStartY, initialStartY + 5.0f, false };
        obstacleSet.push_back(firstObstacle);
    }

    // Gap follows the ramp, starting where the ramp ends
    obstacleSet.push_back(gap(initialStartX + 20.0f, 5.0f, initialStartY + 5.0f));

    // Second obstacle: line(10) at ground level from (25,0) to (35,0)
    {
        std::vector<b2Vec2> vertices;
        vertices.push_back(b2Vec2(initialStartX + 25.0f, initialStartY)); // Starting at the ground level after the gap
        vertices.push_back(b2Vec2(initialStartX + 35.0f, initialStartY)); // Continues horizontally at ground level
        Obstacle secondObstacle = { vertices, initialStartY, initialStartY, false };
        obstacleSet.push_back(secondObstacle);
    }

    return obstacleSet;
}
