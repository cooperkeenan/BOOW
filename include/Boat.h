#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "PhysicsManager.h"
#include "GameState.h"
#include "Obstacle.h" // Make sure you include Obstacle.h

class Boat {
public:
    Boat(b2World& world, PhysicsManager& physicsManager, const sf::Vector2f& position, const sf::Vector2f& size, bool isAIControlled = false); // Constructor for the Boat class
    void update(GameState& currentState); // Updates the boat's state
    void render(sf::RenderWindow& window); // Renders the boat to the window
    void move(float directionX, float directionY, float magnitude); // Moves the boat
    void rotate(float torque); // Rotates the boat
    b2Body* getBoatBody() const; // Returns the Box2D body of the boat
    void setPosition(float x, float y); // Sets the boat's position
    bool checkRespawnNeeded() const; // Checks if the boat needs to be respawned
    void respawnBoat(PhysicsManager& physicsManager, int currentLevel); // Respawns the boat
    void setCurrentLevelData(const std::vector<Obstacle>& levelData); // Sets the level data for the boat
    void setFinishLineX(float x); // Sets the x-coordinate of the finish line
    float getFinishLineX(); // Gets the x-coordinate of the finish line

private:
    sf::RectangleShape boatSprite; // Sprite for rendering the boat
    b2Body* boatBody; // Box2D body for physics simulation
    PhysicsManager& physicsMgr; // Reference to the physics manager
    bool hasCrossedFinishLine; // Flag to track if the boat has crossed the finish line
    float finishLineX; // X-coordinate of the finish line
    std::vector<Obstacle> currentLevelData; // Stores the obstacle data for the current level
};