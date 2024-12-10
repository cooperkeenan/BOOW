#include "Boat.h"
#include "Constants.h"
#include <iostream>
#include <cmath> // For std::abs()

// Constants for speed and rotation caps
constexpr float MAX_SPEED = 0.7f; // Maximum linear velocity
constexpr float MAX_ROTATION_SPEED = 0.2f; // Maximum angular velocity (radians per second)

// Helper function to convert SFML to Box2D coordinates
b2Vec2 sfmlToBox2D(const sf::Vector2f& position) {
    return b2Vec2((position.x - WINDOW_WIDTH / 2.0f) / SCALE, (WINDOW_HEIGHT - position.y) / SCALE);
}

Boat::Boat(b2World& world, PhysicsManager& physicsManager, const sf::Vector2f& position, const sf::Vector2f& size)
    : physicsMgr(physicsManager)
{
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
    boatBody->CreateFixture(&fixtureDef);

    boatSprite.setSize(size);
    boatSprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
    boatSprite.setFillColor(sf::Color::Blue);
}

void Boat::update() {
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

    // Update the sprite position and rotation
    b2Vec2 position = boatBody->GetPosition();
    float angle = boatBody->GetAngle();

    boatSprite.setPosition(position.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - (position.y * SCALE));
    boatSprite.setRotation(-angle * 180.0f / b2_pi);
}

void Boat::render(sf::RenderWindow& window) {
    window.draw(boatSprite);
}

void Boat::move(float directionX, float directionY, float magnitude) {
    // Reduce the magnitude to slow down the movement
    float scaledMagnitude = magnitude * 0.1f; // Scale down by 80%
    b2Vec2 force(directionX * scaledMagnitude, directionY * scaledMagnitude);
    boatBody->ApplyForceToCenter(force, true);
}

void Boat::rotate(float torque) {
    boatBody->ApplyTorque(torque * 0.08f, true); // Scale torque down for smoother rotation
}

b2Body* Boat::getBoatBody() const {
    return boatBody;
}

void Boat::setPosition(float x, float y) {
    boatBody->SetTransform(b2Vec2((x - WINDOW_WIDTH / 2.0f) / SCALE, (WINDOW_HEIGHT - y) / SCALE), boatBody->GetAngle());
    boatBody->SetAwake(true);
}

bool Boat::checkRespawnNeeded() const {
    b2Vec2 pos = boatBody->GetPosition();
    float worldY = WINDOW_HEIGHT - (pos.y * SCALE);
    return (worldY > 1000);
}

void Boat::respawnBoat() {
    boatBody->SetLinearVelocity(b2Vec2(0, 0));
    boatBody->SetAngularVelocity(0.0f);
    setPosition(150, 100.0f);
}
