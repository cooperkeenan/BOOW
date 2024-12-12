#include "Collectable.h"
#include "Constants.h"

// Constructor: Initializes the collectable's physics body and SFML shape.
Collectable::Collectable(b2World& world, const sf::Vector2f& position, float radius)
    : collected(false) {
    // Create Box2D body definition.
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody; // Collectables are static.
    bodyDef.position.Set((position.x - WINDOW_WIDTH / 2.0f) / SCALE, (WINDOW_HEIGHT - position.y) / SCALE); // Convert SFML to Box2D coordinates.
    body = world.CreateBody(&bodyDef); // Create the Box2D body.

    // Create Box2D circle shape.
    b2CircleShape circleShape;
    circleShape.m_radius = radius / 10.0f; // Scale the radius for Box2D.

    // Create Box2D fixture definition.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.isSensor = true; // Make it a sensor so it doesn't cause collisions but detects them.
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this); // Store a pointer to this Collectable object in the fixture's user data.

    // Add collision filtering.
    fixtureDef.filter.categoryBits = CATEGORY_COLLECTABLE; // Set collectable category.
    fixtureDef.filter.maskBits = CATEGORY_PLAYER;        // Only collides with the player.

    body->CreateFixture(&fixtureDef); // Create the fixture on the body.

    // Create SFML shape.
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(radius, radius); // Set origin to the center of the circle.
    shape.setPosition(position);     // Set the position of the SFML shape.
}

// Renders the collectable if it hasn't been collected.
void Collectable::render(sf::RenderWindow& window) {
    if (!collected) { // Only draw if not collected.
        window.draw(shape);
    }
}

// Returns the Box2D body of the collectable.
b2Body* Collectable::getBody() const {
    return body;
}

// Marks the collectable as collected and disables its physics body.
void Collectable::markCollected() {
    collected = true; // Set the collected flag.
    if (body != nullptr) {
        body->SetEnabled(false); // Disable the Box2D body so it no longer triggers collisions.
    }
}

// Returns true if the collectable has been collected, false otherwise.
bool Collectable::isCollected() const {
    return collected;
}