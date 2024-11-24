#include "PhysicsManager.h"
#include "Constants.h"
#include <iostream> // For debugging

// Constructor
PhysicsManager::PhysicsManager()
    : world(b2Vec2(0.0f, 0.0f)),
      timeStep(1.0f / 60.0f),
      velocityIterations(8),
      positionIterations(3),
      groundBody(nullptr)
{
    // Set the contact listener for collision detection
    world.SetContactListener(&contactListener);

    // Define the ground body
    b2BodyDef groundBodyDef;

    // Position the ground at the middle of the screen
    float groundScreenY = WINDOW_HEIGHT / 2.0f; // Screen midpoint
    float groundBox2DY = (WINDOW_HEIGHT - groundScreenY) / SCALE; // Convert to Box2D coordinates
    groundBodyDef.position.Set(0.0f, 0.0f); // Ground position at origin
    groundBody = world.CreateBody(&groundBodyDef);

    // Define a smoother curve with more vertices
    const int numVertices = 20; // Increase for smoother curve
    b2Vec2 vertices[numVertices];

    float startX = -20.0f; // Start of the curve
    float endX = 20.0f;    // End of the curve
    float step = (endX - startX) / (numVertices - 1);

    // Create the curve
    for (int i = 0; i < numVertices; ++i) {
        float x = startX + i * step;
        float y = groundBox2DY + 2.0f * sinf((i / (float)(numVertices - 1)) * b2_pi); // Scaled sine wave
        vertices[i] = b2Vec2(x, y);

        // Debugging log
        std::cout << "Physics Vertex[" << i << "]: (" << vertices[i].x << ", " << vertices[i].y << ")\n";
    }

    // Use b2EdgeShape to create two-sided collision
    for (int i = 0; i < numVertices - 1; ++i) {
        b2EdgeShape edge;
        edge.SetTwoSided(vertices[i], vertices[i + 1]); // Connect consecutive vertices

        b2FixtureDef edgeFixtureDef;
        edgeFixtureDef.shape = &edge;
        edgeFixtureDef.density = 0.0f;
        edgeFixtureDef.friction = 0.3f;
        edgeFixtureDef.restitution = 0.0f; // No bounce
        groundBody->CreateFixture(&edgeFixtureDef);
    }
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
    sf::VertexArray groundShape(sf::LineStrip, 20); // Use line strip for a connected curve

    float offsetX = WINDOW_WIDTH / 2.0f; // Center X
    float offsetY = WINDOW_HEIGHT;      // Bottom Y

    const int numVertices = 20;
    float startX = -20.0f; // Start of the curve
    float endX = 20.0f;    // End of the curve
    float step = (endX - startX) / (numVertices - 1);

    for (int i = 0; i < numVertices; ++i) {
        float x = startX + i * step;
        float y = (WINDOW_HEIGHT / 2.0f) + 20.0f * sinf((i / (float)(numVertices - 1)) * b2_pi); // Same sine wave logic

        groundShape[i].position = sf::Vector2f(
            x * SCALE + offsetX,
            offsetY - y
        );
        groundShape[i].color = sf::Color::Green;

        // Debugging log for render
        std::cout << "Render Vertex[" << i << "]: (" 
                  << groundShape[i].position.x << ", " 
                  << groundShape[i].position.y << ")\n";
    }

    window.draw(groundShape);
}
