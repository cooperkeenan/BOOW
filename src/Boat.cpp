#include "Boat.h"
#include "Constants.h"
#include <iostream>

// Helper function to convert SFML to Box2D coordinates
b2Vec2 sfmlToBox2D(const sf::Vector2f& position) {
    return b2Vec2((position.x - WINDOW_WIDTH / 2.0f) / SCALE, (WINDOW_HEIGHT - position.y) / SCALE);
}

Boat::Boat(b2World& world, const sf::Vector2f& position, const sf::Vector2f& size) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = sfmlToBox2D(position);
    boatBody = world.CreateBody(&bodyDef);

    b2PolygonShape boatShape;
    boatShape.SetAsBox((size.x / 2.0f) / SCALE, (size.y / 2.0f) / SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boatShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.007f;
    fixtureDef.restitution = 0.5f;
    boatBody->CreateFixture(&fixtureDef);

    boatSprite.setSize(size);
    boatSprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
    boatSprite.setFillColor(sf::Color::Blue);
}

void Boat::update() {
    b2Vec2 position = boatBody->GetPosition();
    float angle = boatBody->GetAngle();

    boatSprite.setPosition(position.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - (position.y * SCALE));
    boatSprite.setRotation(-angle * 180.0f / b2_pi);
}

void Boat::render(sf::RenderWindow& window) {
    window.draw(boatSprite);
}

void Boat::move(const b2Vec2& force) {
    boatBody->ApplyForceToCenter(force, true);
}

void Boat::move(float directionX, float directionY, float magnitude) {
    b2Vec2 force(directionX * magnitude, directionY * magnitude);
    boatBody->ApplyForceToCenter(force, true);
}
