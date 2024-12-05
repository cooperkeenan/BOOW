#include "ObstacleManager.h"
#include <cstdlib> // For rand()

// Add an obstacle to the manager
void ObstacleManager::addObstacle(const Obstacle& obstacle) {
    obstacles.push_back(obstacle);
}

// Generate a seamless track from obstacles
void ObstacleManager::generateTrack(std::vector<b2Vec2>& trackVertices, int numObstacles) {
    float currentX = 0.0f;
    float currentHeight = 0.0f;

    for (int i = 0; i < numObstacles; ++i) {
        // Select a random obstacle
        const Obstacle& obstacle = obstacles[rand() % obstacles.size()];

        // Add the obstacle's vertices to the track
        for (const auto& vertex : obstacle.vertices) {
            trackVertices.push_back(b2Vec2(currentX + vertex.x, currentHeight + vertex.y));
        }

        // Update the position for the next obstacle
        currentX += obstacle.vertices.back().x; // Move forward by obstacle's width
        currentHeight += obstacle.endHeight - obstacle.startHeight; // Adjust height
    }
}

// Get the list of predefined obstacles
const std::vector<Obstacle>& ObstacleManager::getObstacles() const {
    return obstacles;
}
