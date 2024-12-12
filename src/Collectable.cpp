#include "Collectable.h"
#include "Constants.h"

// The Collectable class represents an item that the player can pick up.


// Constructor: Creates a static body in the Box2D world and a corresponding SFML shape.
Collectable::Collectable(b2World& world, const sf::Vector2f& position, float radius)
    : collected(false)
{
    // Define the Box2D body at the given position, making it static (no movement).
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set((position.x - WINDOW_WIDTH / 2.0f) / SCALE, (WINDOW_HEIGHT - position.y) / SCALE);
    body = world.CreateBody(&bodyDef);

    // Define a circular shape for the collectable in Box2D.
    b2CircleShape circleShape;
    circleShape.m_radius = radius / 10.0f; // Scale radius to physics units

    // Define fixture properties, making it a sensor so it detects overlaps but doesn't collide physically.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.isSensor = true;
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

    // Set collision filtering so that only the player can "collect" it.
    fixtureDef.filter.categoryBits = CATEGORY_COLLECTABLE;
    fixtureDef.filter.maskBits = CATEGORY_PLAYER; 

    // Create the fixture on the body.
    body->CreateFixture(&fixtureDef);

    // Create the corresponding SFML shape for rendering.
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(radius, radius);  // Center the shape visually
    shape.setPosition(position);
}

// Render the collectable if it hasn't been collected yet.
void Collectable::render(sf::RenderWindow& window) {
    if (!collected) {
        window.draw(shape);
    }
}

// Accessor for the underlying Box2D body.
b2Body* Collectable::getBody() const {
    return body;
}

// Mark the collectable as collected. Disable its physics body so it no longer appears or interacts.
void Collectable::markCollected() {
    collected = true;
    if (body != nullptr) {
        body->SetEnabled(false); 
    }
}

// Check if this collectable is already collected.
bool Collectable::isCollected() const {
    return collected;
}
