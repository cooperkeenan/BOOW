#include "Boat.h"
#include "Constants.h"
#include "GameState.h"
#include "levels.h"
#include "BuildObstacles.h"
#include <iostream>
#include <cmath>
#include <SFML/System/Vector2.hpp>

// Constants for speed and rotation limitations.
constexpr float MAX_SPEED = 0.5f;        // Maximum linear speed of the boat.
constexpr float MAX_ROTATION_SPEED = 0.1f; // Maximum angular speed of the boat.

// Helper function to convert SFML coordinates to Box2D coordinates.
b2Vec2 sfmlToBox2D(const sf::Vector2f& position) {
    return b2Vec2((position.x - WINDOW_WIDTH / 2.0f) / SCALE, (WINDOW_HEIGHT - position.y) / SCALE);
}

// Constructor: Initializes the boat's physics body and sprite.
Boat::Boat(b2World& world, PhysicsManager& physicsManager, const sf::Vector2f& position, const sf::Vector2f& size, bool isAIControlled)
    : physicsMgr(physicsManager), hasCrossedFinishLine(false), currentLevelData() { // Initialize currentLevelData
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = sfmlToBox2D(position);
    boatBody = world.CreateBody(&bodyDef);

    b2PolygonShape boatShape;
    boatShape.SetAsBox((size.x / 2.0f) / SCALE, (size.y / 2.0f) / SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boatShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.005f;
    fixtureDef.restitution = 0.5f;

    // Set collision filtering based on whether the boat is AI-controlled or player-controlled.
    if (isAIControlled) {
        fixtureDef.filter.categoryBits = CATEGORY_AI;
        fixtureDef.filter.maskBits = CATEGORY_OBSTACLE;
    } else {
        fixtureDef.filter.categoryBits = CATEGORY_PLAYER;
        fixtureDef.filter.maskBits = CATEGORY_OBSTACLE | CATEGORY_COLLECTABLE;
    }

    boatBody->CreateFixture(&fixtureDef);

    boatSprite.setSize(size);
    boatSprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
    boatSprite.setFillColor(isAIControlled ? sf::Color::Red : sf::Color::Blue);
}

// Sets the current level's obstacle data for finish line calculations.
void Boat::setCurrentLevelData(const std::vector<Obstacle>& levelData) {
    currentLevelData = levelData;
}

// Gets the x-coordinate of the finish line based on the current level's data.
float Boat::getFinishLineX() {
    if (currentLevel == 1) {
        return 1300.0f; //hard coded finishline for level 1
    } else if (currentLevel == 2) {
        return 5000.0f; //hard coded finishline for level 2

        const Obstacle& lastObstacle = currentLevelData.back();

        if (lastObstacle.vertices.empty()) {
            return 0.0f;
        }

        float maxX = lastObstacle.vertices[0].x;
        for (const b2Vec2& vertex : lastObstacle.vertices) { // Iterate over b2Vec2
            if (vertex.x > maxX) {
                maxX = vertex.x;
            }
        }
        return maxX + 10.0f;
    }
    return 0.0f;
}

// Updates the boat's state (position, rotation, velocity) based on physics and game logic.
void Boat::update(GameState& currentState) {
    b2Vec2 velocity = boatBody->GetLinearVelocity();
    float speed = velocity.Length();
    // Cap the boat's speed.
    if (speed > MAX_SPEED) {
        b2Vec2 cappedVelocity = velocity;
        cappedVelocity *= MAX_SPEED / speed;
        boatBody->SetLinearVelocity(cappedVelocity);
    }

    float angularVelocity = boatBody->GetAngularVelocity();
    // Cap the boat's rotation speed.
    if (std::abs(angularVelocity) > MAX_ROTATION_SPEED) {
        float cappedAngularVelocity = (angularVelocity > 0 ? 1 : -1) * MAX_ROTATION_SPEED;
        boatBody->SetAngularVelocity(cappedAngularVelocity);
    }

    b2Vec2 position = boatBody->GetPosition();
    float angle = boatBody->GetAngle();
    // Update the boat's sprite position and rotation based on the physics body.
    boatSprite.setPosition(position.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - (position.y * SCALE));
    boatSprite.setRotation(-angle * 180.0f / b2_pi);

    // Check if the boat has crossed the finish line.
    if (!hasCrossedFinishLine) {
        float finishLinePosition = getFinishLineX();
        if (position.x >= finishLinePosition / SCALE) { // Correct scaling
            hasCrossedFinishLine = true;
            currentState = GameState::LevelComplete;
            std::cout << "Boat has crossed the finish line on level " << currentLevel << "!" << std::endl;
        }
    }
}

// Renders the boat to the specified window.
void Boat::render(sf::RenderWindow& window) {
    window.draw(boatSprite);
}

// Applies a force to move the boat.
void Boat::move(float directionX, float directionY, float magnitude) {
    float scaledMagnitude = magnitude * 0.1f;
    b2Vec2 force(directionX * scaledMagnitude, directionY * scaledMagnitude);
    boatBody->ApplyForceToCenter(force, true);
}

// Applies a torque to rotate the boat.
void Boat::rotate(float torque) {
    float scaledTorque = torque * 0.08f;
    boatBody->ApplyTorque(scaledTorque, true);
}

// Returns the boat's Box2D body.
b2Body* Boat::getBoatBody() const {
    return boatBody;
}

// Sets the boat's position in the Box2D world.
void Boat::setPosition(float x, float y) {
    boatBody->SetTransform(b2Vec2((x - WINDOW_WIDTH / 2.0f) / SCALE, (WINDOW_HEIGHT - y) / SCALE), boatBody->GetAngle());
    boatBody->SetAwake(true);
}

// Sets the x-coordinate of the finish line.
void Boat::setFinishLineX(float x) {
    finishLineX = x;
}

// Checks if the boat needs to be respawned (e.g., if it falls off the screen).
bool Boat::checkRespawnNeeded() const {
    b2Vec2 pos = boatBody->GetPosition();
    float worldY = WINDOW_HEIGHT - (pos.y * SCALE);
    return (worldY > 1000);
}

// Respawns the boat to its starting position and resets its physics state.
void Boat::respawnBoat(PhysicsManager& physicsManager, int currentLevel) {
    boatBody->SetLinearVelocity(b2Vec2(0, 0));
    boatBody->SetAngularVelocity(0.0f);
    setPosition(150, 100.0f);
    boatBody->SetAwake(true);
    hasCrossedFinishLine = false;

    if (currentLevel == 1) {
        physicsManager.reset(level_1());
        setCurrentLevelData(level_1()); // Set level data after reset
    } else if (currentLevel == 2) {
        physicsManager.reset(level_2());
        setCurrentLevelData(level_2()); // Set level data after reset
    }
}