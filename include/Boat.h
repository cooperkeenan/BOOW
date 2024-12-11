#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "PhysicsManager.h"
#include "GameState.h"

class Boat {
public:
    Boat(b2World& world, PhysicsManager& physicsManager, const sf::Vector2f& position, const sf::Vector2f& size, bool isAIControlled = false);
    void update(GameState& currentState);
    void render(sf::RenderWindow& window);
    void move(float directionX, float directionY, float magnitude);
    void rotate(float torque);
    b2Body* getBoatBody() const;
    void setPosition(float x, float y);
    bool checkRespawnNeeded() const;
    void respawnBoat(PhysicsManager& physicsManager);

private:
    sf::RectangleShape boatSprite;
    b2Body* boatBody;
    PhysicsManager& physicsMgr;
    bool hasCrossedFinishLine;
};
