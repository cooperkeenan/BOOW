#include "Boat.h"
#include "Constants.h"
#include "GameState.h"
#include "levels.h"
#include "BuildObstacles.h"
#include <iostream>
#include <cmath> // For std::abs()

// Constants for speed and rotation caps
constexpr float MAX_SPEED = 0.7f; // Maximum linear velocity
constexpr float MAX_ROTATION_SPEED = 0.2f; // Maximum angular velocity (radians per second)

// Collision categories

// Helper function to convert SFML coordinates to Box2D
b2Vec2 sfmlToBox2D(const sf::Vector2f& position) {
    return b2Vec2((position.x - WINDOW_WIDTH / 2.0f) / SCALE, (WINDOW_HEIGHT - position.y) / SCALE);
}

// Boat constructor
Boat::Boat(b2World& world, PhysicsManager& physicsManager, const sf::Vector2f& position, const sf::Vector2f& size, bool isAIControlled)
    : physicsMgr(physicsManager), hasCrossedFinishLine(false) {
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

    // Set collision filtering
    if (isAIControlled) {
        fixtureDef.filter.categoryBits = CATEGORY_AI; // AI boat
        fixtureDef.filter.maskBits = CATEGORY_OBSTACLE; // Collides only with obstacles
    } else {
        fixtureDef.filter.categoryBits = CATEGORY_PLAYER; // Player boat
        fixtureDef.filter.maskBits = CATEGORY_OBSTACLE | CATEGORY_COLLECTABLE; // Collides only with obstacles
    }

    boatBody->CreateFixture(&fixtureDef);

    boatSprite.setSize(size);
    boatSprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
    boatSprite.setFillColor(isAIControlled ? sf::Color::Red : sf::Color::Blue);
}

// Update the boat's state
void Boat::update(GameState& currentState) {
    // Cap linear speed
    b2Vec2 velocity = boatBody->GetLinearVelocity();
    float speed = velocity.Length();
    if (speed > MAX_SPEED) {
        b2Vec2 cappedVelocity = velocity;
        cappedVelocity *= MAX_SPEED / speed; // Scale down to max speed
        boatBody->SetLinearVelocity(cappedVelocity);
    }

    // Cap angular speed
    float angularVelocity = boatBody->GetAngularVelocity();
    if (std::abs(angularVelocity) > MAX_ROTATION_SPEED) {
        float cappedAngularVelocity = (angularVelocity > 0 ? 1 : -1) * MAX_ROTATION_SPEED;
        boatBody->SetAngularVelocity(cappedAngularVelocity);
    }

    // Update sprite position and rotation
    b2Vec2 position = boatBody->GetPosition();
    float angle = boatBody->GetAngle();
    boatSprite.setPosition(position.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - (position.y * SCALE));
    boatSprite.setRotation(-angle * 180.0f / b2_pi);

    // Check if boat crossed the finish line
    float finishLinePosition = getFinishLineX();
    if (!hasCrossedFinishLine && position.x >= finishLinePosition) {
        std::cout << "Boat has crossed the finish line!" << std::endl;
        hasCrossedFinishLine = true;
        currentState = GameState::LevelComplete;
    }
}

// Render the boat
void Boat::render(sf::RenderWindow& window) {
    window.draw(boatSprite);
}

// Move the boat by applying a force
void Boat::move(float directionX, float directionY, float magnitude) {
    float scaledMagnitude = magnitude * 0.1f; // Scale down by 80%
    b2Vec2 force(directionX * scaledMagnitude, directionY * scaledMagnitude);
    boatBody->ApplyForceToCenter(force, true);
}

void Boat::rotate(float torque) {
    float scaledTorque = torque * 0.08f; // Scale torque down
    boatBody->ApplyTorque(scaledTorque, true);
}

// Get the Box2D body of the boat
b2Body* Boat::getBoatBody() const {
    return boatBody;
}

// Set the position of the boat
void Boat::setPosition(float x, float y) {
    boatBody->SetTransform(b2Vec2((x - WINDOW_WIDTH / 2.0f) / SCALE, (WINDOW_HEIGHT - y) / SCALE), boatBody->GetAngle());
    boatBody->SetAwake(true);
}

// Check if respawn is needed
bool Boat::checkRespawnNeeded() const {
    b2Vec2 pos = boatBody->GetPosition();
    float worldY = WINDOW_HEIGHT - (pos.y * SCALE);
    return (worldY > 1000);
}

// Respawn the boat
void Boat::respawnBoat(PhysicsManager& physicsManager, int currentLevel) {
    boatBody->SetLinearVelocity(b2Vec2(0, 0));
    boatBody->SetAngularVelocity(0.0f);
    setPosition(150, 100.0f);
    boatBody->SetAwake(true);
    hasCrossedFinishLine = false;

    if (currentLevel == 1) {
        physicsManager.reset(level_1());
    } else if (currentLevel == 2) {
        physicsManager.reset(level_2());
    }
    /*
    else if (currentLevel == 3) {
        physicsManager.reset(level_3());
    }
    */
}

