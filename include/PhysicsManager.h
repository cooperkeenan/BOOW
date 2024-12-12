#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "ObstacleManager.h"
#include "Constants.h"
#include "Collectable.h"

// Manages the Box2D physics world and related elements.
class PhysicsManager {
public:
    PhysicsManager();                                                  // Constructor: Initializes the physics world.
    b2World& getWorld();                                                // Returns a reference to the Box2D world.
    void step();                                                       // Advances the physics simulation by one time step.
    void applyGravity(const b2Vec2& gravity);                         // Applies a given gravity vector to the world.
    void applyGravityIfNeeded(bool& gravityApplied, float elapsedTime, float triggerTime); // Applies gravity after a specified delay.
    void renderGround(sf::RenderWindow& window);                      // Renders the ground (obstacles) to the window.
    void renderCollectables(sf::RenderWindow& window);                // Renders the collectable items to the window.
    int checkCollectables();                                           // Checks for collected collectables and updates the score.
    void reset(const std::vector<Obstacle>& selected_level);          // Resets the physics world and obstacles for a new level.
    bool gravityApplied;                                               // Flag indicating if gravity has been applied.
    sf::Clock clock;                                                   // Clock for timing events within the physics manager.

private:
    void initializeObstacles(const std::vector<Obstacle>& selected_level); // Initializes obstacles from level data.
    void createFixturesFromObstacle(const Obstacle& obs);                // Creates Box2D fixtures for a given obstacle.

    b2World world;                // The Box2D physics world.
    float timeStep;               // Time step for the physics simulation.
    int32 velocityIterations;     // Number of velocity iterations for the Box2D solver.
    int32 positionIterations;     // Number of position iterations for the Box2D solver.
    b2Body* groundBody;           // The static ground body in the physics world.
    ObstacleManager obstacleManager; // Manages the obstacles in the physics world.
    std::vector<Collectable> collectables; // Stores the collectable items.
};