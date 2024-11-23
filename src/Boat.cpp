#include "Boat.h"

Boat::Boat(b2World& world, const sf::Vector2f& position, const sf::Vector2f& size) {
    // Setup Box2D body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x / 30.0f, position.y / 30.0f);
    boatBody = world.CreateBody(&bodyDef);

    b2PolygonShape boatShape;
    boatShape.SetAsBox(size.x / 60.0f, size.y / 60.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boatShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    boatBody->CreateFixture(&fixtureDef);

    // Setup SFML sprite
    boatSprite.setSize(size);
    boatSprite.setOrigin(size.x / 2, size.y / 2);
    boatSprite.setFillColor(sf::Color::Blue);
}

void Boat::update() {
    // Sync SFML sprite with Box2D body
    b2Vec2 position = boatBody->GetPosition();
    float angle = boatBody->GetAngle();

    boatSprite.setPosition(position.x * 30.0f, position.y * 30.0f);
    boatSprite.setRotation(angle * 180.0f / b2_pi);
}

void Boat::render(sf::RenderWindow& window) {
    window.draw(boatSprite);
}
