#include "Boat.h"
#include "Constants.h"
#include <iostream>

Boat::Boat(b2World& world, const sf::Vector2f& position, const sf::Vector2f& size) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set((position.x - WINDOW_WIDTH / 2.0f) / SCALE, (WINDOW_HEIGHT - position.y) / SCALE);
    boatBody = world.CreateBody(&bodyDef);

    b2PolygonShape boatShape;
    boatShape.SetAsBox((size.x / 2.0f) / SCALE, (size.y / 2.0f) / SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boatShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
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
