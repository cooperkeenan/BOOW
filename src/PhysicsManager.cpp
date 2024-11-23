// PhysicsManager.cpp
#include "PhysicsManager.h"
#include "Constants.h"

// Constructor
PhysicsManager::PhysicsManager()
    : world(b2Vec2(0.0f, 0.0f)),
      timeStep(1.0f / 60.0f),
      velocityIterations(8),
      positionIterations(3),
      groundBody(nullptr),
      groundThickness(0.1f)
{
    // Set the contact listener for collision detection
    world.SetContactListener(&contactListener);

    // Define the ground body
    b2BodyDef groundBodyDef;

    // Position the ground at the middle of the screen
    float groundScreenY = WINDOW_HEIGHT / 2.0f;
    float groundBox2DY = (WINDOW_HEIGHT - groundScreenY) / SCALE;
    groundBodyDef.position.Set(0.0f, groundBox2DY);
    groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground shape (a horizontal line)
    b2PolygonShape groundBox;
    groundBox.SetAsBox(20.0f, groundThickness); // Half-width and half-height in Box2D units

    // Attach the shape to the body
    groundBody->CreateFixture(&groundBox, 0.0f);
}

// Define the applyGravity function
void PhysicsManager::applyGravity(const b2Vec2& gravity) {
    world.SetGravity(gravity);

    // Wake up all bodies in the world
    for (b2Body* body = world.GetBodyList(); body; body = body->GetNext()) {
        body->SetAwake(true);
    }
}


// Define the getWorld function
b2World& PhysicsManager::getWorld() {
    return world;
}

// Define the step function
void PhysicsManager::step() {
    world.Step(timeStep, velocityIterations, positionIterations);
}


// PhysicsManager.cpp
void PhysicsManager::renderGround(sf::RenderWindow& window) {
    float groundWidthBox2D = 40.0f;  // Total width in Box2D units
    float groundHeightBox2D = groundThickness * 2.0f;   // Total height in Box2D units

    float groundWidth = groundWidthBox2D * SCALE;
    float groundHeight = groundHeightBox2D * SCALE;

    sf::RectangleShape ground(sf::Vector2f(groundWidth, groundHeight));
    ground.setFillColor(sf::Color::Green);
    ground.setOrigin(groundWidth / 2.0f, groundHeight / 2.0f);

    // Get the ground body's position
    b2Vec2 position = groundBody->GetPosition();
    float x = position.x * SCALE + WINDOW_WIDTH / 2.0f;
    float y = WINDOW_HEIGHT - (position.y * SCALE);

    ground.setPosition(x, y);
    window.draw(ground);
}
