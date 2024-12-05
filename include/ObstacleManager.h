#ifndef OBSTACLEMANAGER_H
#define OBSTACLEMANAGER_H

#include <vector>
#include <Box2D/Box2D.h>

// Define the Obstacle struct
struct Obstacle {
    std::vector<b2Vec2> vertices; // Vertices of the obstacle
    float startHeight;            // Starting height
    float endHeight;              // Ending height
};

// Define the ObstacleManager class
class ObstacleManager {
public:
    void addObstacle(const Obstacle& obstacle); // Add an obstacle
    void generateTrack(std::vector<b2Vec2>& trackVertices, int numObstacles); // Generate a track
    const std::vector<Obstacle>& getObstacles() const; // Get the list of obstacles

private:
    std::vector<Obstacle> obstacles; // List of obstacles
};

#endif // OBSTACLEMANAGER_H
