#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "PhysicsManager.h"

class Boat {
public:
    Boat(b2World& world, PhysicsManager& physicsManager, const sf::Vector2f& position, const sf::Vector2f& size, bool isAIControlled = false);
    void update();
    void render(sf::RenderWindow& window);
    void move(float directionX, float directionY, float magnitude);
    void rotate(float torque); // New method for rotation
    b2Body* getBoatBody() const;
    bool checkRespawnNeeded() const;
    void respawnBoat();

private:
    sf::RectangleShape boatSprite;
    b2Body* boatBody;
    void setPosition(float x, float y);
    PhysicsManager& physicsMgr;
};