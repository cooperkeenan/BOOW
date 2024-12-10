#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "ObstacleManager.h"
#include "Constants.h"
#include "Collectable.h"

class PhysicsManager {
public:
    PhysicsManager();
    b2World& getWorld();
    void step();
    void applyGravity(const b2Vec2& gravity);
    void applyGravityIfNeeded(bool& gravityApplied, float elapsedTime, float triggerTime);
    void renderGround(sf::RenderWindow& window);
    void renderCollectables(sf::RenderWindow& window);
    int checkCollectables();
    bool gravityApplied;
    sf::Clock clock;


private:
    void initializeObstacles(const std::vector<Obstacle>& selected_level);
    void createFixturesFromObstacle(const Obstacle& obs);

    b2World world;
    float timeStep;
    int32 velocityIterations;
    int32 positionIterations;
    b2Body* groundBody;
    ObstacleManager obstacleManager;
    std::vector<Collectable> collectables;
};