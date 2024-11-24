#include "PhysicsManager.h"
#include "Constants.h"

// Constructor
PhysicsManager::PhysicsManager()
    : world(b2Vec2(0.0f, 0.0f)),
      timeStep(1.0f / 60.0f),
      velocityIterations(8),
      positionIterations(3),
      groundBody(nullptr),
      groundThickness(0.2f) // Increased thickness for the polygon
{
    // Set the contact listener for collision detection
    world.SetContactListener(&contactListener);

    // Define the ground body
    b2BodyDef groundBodyDef;

    // Position the ground at the middle of the screen
    float groundScreenY = WINDOW_HEIGHT / 2.0f;
    float groundBox2DY = (WINDOW_HEIGHT - groundScreenY) / SCALE; // Y-position in Box2D coordinates
    groundBodyDef.position.Set(0.0f, 0.0f); // Ground position at origin
    groundBody = world.CreateBody(&groundBodyDef);

    // Define a polygonal slope for the curved ground
    b2Vec2 vertices[] = {
        b2Vec2(-20.0f, groundBox2DY),               // Start point (left-bottom)
        b2Vec2(-10.0f, groundBox2DY + 2.0f),        // Curve upward
        b2Vec2(0.0f, groundBox2DY + 5.0f),          // Peak of the curve
        b2Vec2(10.0f, groundBox2DY + 2.0f),         // Curve downward
        b2Vec2(20.0f, groundBox2DY),                // End point (right-bottom)
        b2Vec2(20.0f, groundBox2DY - groundThickness), // Right-bottom (thickness)
        b2Vec2(0.0f, groundBox2DY - groundThickness), // Middle-bottom (thickness)
        b2Vec2(-20.0f, groundBox2DY - groundThickness) // Left-bottom (thickness)
    };

    b2PolygonShape groundPolygon;
    groundPolygon.Set(vertices, 8); // Set the polygon with 8 vertices

    // Attach the polygon shape to the body
    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundPolygon;
    groundFixtureDef.density = 0.0f;
    groundFixtureDef.friction = 0.3f;
    groundFixtureDef.restitution = 0.0f; // No bounce
    groundBody->CreateFixture(&groundFixtureDef);

    // Add invisible world boundaries (left, right, and top edges)
    b2BodyDef boundaryBodyDef;
    b2Body* boundaryBody = world.CreateBody(&boundaryBodyDef);

    b2EdgeShape boundaryEdge;

    // Left boundary
    boundaryEdge.SetTwoSided(
        b2Vec2(-WINDOW_WIDTH / 2.0f / SCALE, 0.0f),
        b2Vec2(-WINDOW_WIDTH / 2.0f / SCALE, WINDOW_HEIGHT / SCALE)
    );
    boundaryBody->CreateFixture(&boundaryEdge, 0.0f);

    // Right boundary
    boundaryEdge.SetTwoSided(
        b2Vec2(WINDOW_WIDTH / 2.0f / SCALE, 0.0f),
        b2Vec2(WINDOW_WIDTH / 2.0f / SCALE, WINDOW_HEIGHT / SCALE)
    );
    boundaryBody->CreateFixture(&boundaryEdge, 0.0f);

    // Top boundary
    boundaryEdge.SetTwoSided(
        b2Vec2(-WINDOW_WIDTH / 2.0f / SCALE, WINDOW_HEIGHT / SCALE),
        b2Vec2(WINDOW_WIDTH / 2.0f / SCALE, WINDOW_HEIGHT / SCALE)
    );
    boundaryBody->CreateFixture(&boundaryEdge, 0.0f);
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

// Render the ground (curved slope)
void PhysicsManager::renderGround(sf::RenderWindow& window) {
    sf::VertexArray groundShape(sf::LineStrip, 5); // Use line strip for a connected curve

    float offsetX = WINDOW_WIDTH / 2.0f;
    float offsetY = WINDOW_HEIGHT;

    // Define the same points used in the polygon shape
    float groundScreenY = WINDOW_HEIGHT / 2.0f;
    float groundBox2DY = (WINDOW_HEIGHT - groundScreenY) / SCALE; // Y-position in Box2D coordinates

    b2Vec2 vertices[] = {
        b2Vec2(-20.0f, groundBox2DY),
        b2Vec2(-10.0f, groundBox2DY + 2.0f),
        b2Vec2(0.0f, groundBox2DY + 5.0f),
        b2Vec2(10.0f, groundBox2DY + 2.0f),
        b2Vec2(20.0f, groundBox2DY)
    };

    for (int i = 0; i < 5; ++i) {
        groundShape[i].position = sf::Vector2f(
            vertices[i].x * SCALE + offsetX,
            offsetY - vertices[i].y * SCALE
        );
        groundShape[i].color = sf::Color::Green;
    }

    window.draw(groundShape);
}
