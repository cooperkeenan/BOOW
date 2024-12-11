#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "PhysicsManager.h"
#include "GameState.h"

class Boat {
public:
    Boat(b2World& world, PhysicsManager& physicsManager, const sf::Vector2f& position, const sf::Vector2f& size);
    void update(GameState &currentState);
    void render(sf::RenderWindow& window);
    void move(float directionX, float directionY, float magnitude);
    b2Body* getBoatBody() const;
    bool checkRespawnNeeded() const;
    void respawnBoat(PhysicsManager& physicsManager);
    void setPosition(float x, float y); // Declare the setPosition method
    void rotate(float torque);
    

private:
    sf::RectangleShape boatSprite;
    b2Body* boatBody;
    PhysicsManager& physicsMgr;
    bool hasCrossedFinishLine = false;
};
