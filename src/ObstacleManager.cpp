#include "ObstacleManager.h"
#include <cmath>

void ObstacleManager::addObstacle(const Obstacle& obstacle) {
    obstacles.push_back(obstacle);
}

const std::vector<Obstacle>& ObstacleManager::getObstacles() const {
    return obstacles;
}

void ObstacleManager::generateTrack(std::vector<b2Vec2>& trackVertices, int numObstacles) {
    float currentX = 0.0f;
    float currentHeight = 0.0f;

    for (int i = 0; i < numObstacles; ++i) {
        if (obstacles.empty()) break;

        Obstacle obstacle = obstacles[i % obstacles.size()];
        float heightOffset = currentHeight - obstacle.startHeight;

        if (obstacle.vertices.size() == 1) {
            b2Vec2 v = obstacle.vertices[0];

            if (std::isnan(v.x) || std::isnan(v.y)) {
                // BREAK OBSTACLE
                // Do not add any vertex, just a marker to skip edges.
                // No change to currentX or currentHeight
                continue;
            } else {
                // GAP OBSTACLE
                float gapLength = v.x;
                currentX += gapLength;
                // No vertices added
                continue;
            }
        } else if (obstacle.vertices.size() > 1) {
            // NORMAL OBSTACLE
            for (auto& vertex : obstacle.vertices) {
                trackVertices.push_back(b2Vec2(currentX + vertex.x, vertex.y + heightOffset));
            }
            float lastX = obstacle.vertices.back().x;
            currentX += lastX;
            currentHeight = obstacle.endHeight + heightOffset;
        } else {
            // Empty obstacle
            continue;
        }
    }
}

