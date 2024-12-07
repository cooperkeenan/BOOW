#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "ObstacleManager.h"
#include "Constants.h"

class PhysicsManager {
public:
    PhysicsManager();
    b2World& getWorld();
    void step();
    void applyGravity(const b2Vec2& gravity);
    void applyGravityIfNeeded(bool& gravityApplied, float elapsedTime, float triggerTime);
    void renderGround(sf::RenderWindow& window);

private:
    void initializeObstacles();
    void createFixturesFromTrack(); // New method to create fixtures from trackVertices

    b2World world;
    float timeStep;
    int32 velocityIterations;
    int32 positionIterations;
    b2Body* groundBody;
    std::vector<b2Vec2> trackVertices;
    ObstacleManager obstacleManager;
};

#endif // PHYSICSMANAGER_H
