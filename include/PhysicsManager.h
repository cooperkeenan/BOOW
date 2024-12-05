#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "ObstacleManager.h" // Include ObstacleManager

class PhysicsManager {
public:
    PhysicsManager();
    b2World& getWorld();                            // Access the Box2D world
    void step();                                    // Step the physics simulation
    void applyGravity(const b2Vec2& gravity);       // Apply global gravity
    void applyGravityIfNeeded(bool& gravityApplied, float elapsedTime, float triggerTime);
    void renderGround(sf::RenderWindow& window);    // Render the track (ground)

private:
    void initializeObstacles();                     // Initialize predefined obstacles
    void generateTrack();                           // Generate the track using obstacles

    b2World world;                                  // Box2D world
    float timeStep;                                 // Time step for physics simulation
    int32 velocityIterations;                      // Velocity solver iterations
    int32 positionIterations;                      // Position solver iterations
    b2Body* groundBody;                             // Ground body for the track
    std::vector<b2Vec2> trackVertices;              // Vertices for the ground track
    ObstacleManager obstacleManager;                // Manage obstacles
};

#endif // PHYSICSMANAGER_H
