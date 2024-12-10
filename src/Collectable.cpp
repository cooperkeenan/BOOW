#include "Collectable.h"
#include "Constants.h"

Collectable::Collectable(b2World& world, const sf::Vector2f& position, float radius)
    : collected(false) {
    // Create Box2D body
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set((position.x - WINDOW_WIDTH / 2.0f) / SCALE, (WINDOW_HEIGHT - position.y) / SCALE);
    body = world.CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = radius / 10.0f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.isSensor = true;
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    body->CreateFixture(&fixtureDef);

    // Create SFML shape
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(radius, radius);
    shape.setPosition(position);
}

void Collectable::render(sf::RenderWindow& window) {
    if (!collected) {
        window.draw(shape);
    }
}

b2Body* Collectable::getBody() const {
    return body;
}

void Collectable::markCollected() {
    collected = true;
    if (body != nullptr) {
        body->SetEnabled(false); 
    }
}

bool Collectable::isCollected() const {
    return collected;
}
