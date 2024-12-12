#include "Boat.h"
#include "Constants.h"
#include "GameState.h"
#include "levels.h"
#include "BuildObstacles.h"
#include <iostream>
#include <cmath> 


// Constants for speed and rotation caps
constexpr float MAX_SPEED = 0.6f;         
constexpr float MAX_ROTATION_SPEED = 0.1f; 


// This adjusts for the coordinate system differences between SFML and Box2D.
b2Vec2 sfmlToBox2D(const sf::Vector2f& position) {
    return b2Vec2((position.x - WINDOW_WIDTH / 2.0f) / SCALE, (WINDOW_HEIGHT - position.y) / SCALE);
}

// Boat constructor
Boat::Boat(b2World& world, PhysicsManager& physicsManager, const sf::Vector2f& position, const sf::Vector2f& size, bool isAIControlled)
    : physicsMgr(physicsManager), hasCrossedFinishLine(false)
{
    // Define a dynamic body for the boat
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = sfmlToBox2D(position);
    boatBody = world.CreateBody(&bodyDef);

    // Define boat shape 
    b2PolygonShape boatShape;
    boatShape.SetAsBox((size.x / 2.0f) / SCALE, (size.y / 2.0f) / SCALE);

    // Define fixture properties (density, friction, etc.)
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boatShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.007f;   
    fixtureDef.restitution = 0.5f;  

    // Collision filtering for player/AI distinctions
    if (isAIControlled) {
        fixtureDef.filter.categoryBits = CATEGORY_AI; 
        fixtureDef.filter.maskBits = CATEGORY_OBSTACLE; 
    } else {
        fixtureDef.filter.categoryBits = CATEGORY_PLAYER; 
        fixtureDef.filter.maskBits = CATEGORY_OBSTACLE | CATEGORY_COLLECTABLE; 
    }

    // Attach fixture to the boat body
    boatBody->CreateFixture(&fixtureDef);

    // Initialize SFML sprite for visual representation
    boatSprite.setSize(size);
    boatSprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
    boatSprite.setFillColor(isAIControlled ? sf::Color::Red : sf::Color::Blue);
}

// Update the boat's state each frame
void Boat::update(GameState& currentState) {
    // Enforce linear speed limit
    b2Vec2 velocity = boatBody->GetLinearVelocity();
    float speed = velocity.Length();
    if (speed > MAX_SPEED) {
        b2Vec2 cappedVelocity = velocity;
        cappedVelocity *= MAX_SPEED / speed;
        boatBody->SetLinearVelocity(cappedVelocity);
    }

    // Enforce angular speed limit
    float angularVelocity = boatBody->GetAngularVelocity();
    if (std::abs(angularVelocity) > MAX_ROTATION_SPEED) {
        float cappedAngularVelocity = (angularVelocity > 0 ? 1 : -1) * MAX_ROTATION_SPEED;
        boatBody->SetAngularVelocity(cappedAngularVelocity);
    }

    // Update SFML sprite position and rotation to match Box2D body
    b2Vec2 position = boatBody->GetPosition();
    float angle = boatBody->GetAngle();
    boatSprite.setPosition(position.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - (position.y * SCALE));
    boatSprite.setRotation(-angle * 180.0f / b2_pi);

    // Check if the boat has crossed the finish line
    float finishLinePosition = getFinishLineX();
    if (!hasCrossedFinishLine && position.x >= finishLinePosition) {
        hasCrossedFinishLine = true;

        // Handle level-specific finish line logic
        if (currentLevel == 2) {
            std::cout << "Boat has crossed the finish line on level 2!" << std::endl;
        } else {
            std::cout << "Boat has crossed the finish line!" << std::endl;
            //currentState = GameState::LevelComplete; // Uncomment if needed
        }
    }
}

// Render the boat onto the SFML window
void Boat::render(sf::RenderWindow& window) {
    window.draw(boatSprite);
}

// Apply a linear force to move the boat in a given direction
void Boat::move(float directionX, float directionY, float magnitude) {
    float scaledMagnitude = magnitude * 0.1f; 
    b2Vec2 force(directionX * scaledMagnitude, directionY * scaledMagnitude);
    boatBody->ApplyForceToCenter(force, true);
}

// Apply torque to rotate the boat
void Boat::rotate(float torque) {
    float scaledTorque = torque * 0.08f;
    boatBody->ApplyTorque(scaledTorque, true);
}

// Return the Box2D body representing the boat
b2Body* Boat::getBoatBody() const {
    return boatBody;
}

// Set the boat's position to a given point in SFML coordinates
void Boat::setPosition(float x, float y) {
    boatBody->SetTransform(b2Vec2((x - WINDOW_WIDTH / 2.0f) / SCALE, (WINDOW_HEIGHT - y) / SCALE), boatBody->GetAngle());
    boatBody->SetAwake(true);
}

// Check if the boat should be respawned
bool Boat::checkRespawnNeeded() const {
    b2Vec2 pos = boatBody->GetPosition();
    float worldY = WINDOW_HEIGHT - (pos.y * SCALE);
    return (worldY > 1000);
}

// Respawn the boat and optionally reset the level
void Boat::respawnBoat(PhysicsManager& physicsManager, int currentLevel) {
    boatBody->SetLinearVelocity(b2Vec2(0, 0));
    boatBody->SetAngularVelocity(0.0f);
    setPosition(150, 100.0f);
    boatBody->SetAwake(true);
    hasCrossedFinishLine = false;

    // Reset the physics world with the specified level's obstacles
    if (currentLevel == 1) {
        physicsManager.reset(level_1());
    } else if (currentLevel == 2) {
        physicsManager.reset(level_2());
    }
}
