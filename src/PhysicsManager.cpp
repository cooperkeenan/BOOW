#include "PhysicsManager.h"
#include "Constants.h"
#include <iostream>

// Constructor
PhysicsManager::PhysicsManager()
    : world(b2Vec2(0.0f, 0.0f)),
      timeStep(1.0f / 60.0f),
      velocityIterations(8),
      positionIterations(3),
      groundBody(nullptr)
{
    // Define the ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 0.0f);
    groundBody = world.CreateBody(&groundBodyDef);

    // Generate curve vertices
    generateCurveVertices(100, -20.0f, 20.0f);

    // Use b2EdgeShape for the physics boundary
    for (size_t i = 0; i < curveVertices.size() - 1; ++i) {
        b2EdgeShape edge;
        edge.SetTwoSided(curveVertices[i], curveVertices[i + 1]); 

        b2FixtureDef edgeFixtureDef;
        edgeFixtureDef.shape = &edge;
        edgeFixtureDef.density = 0.0f;
        edgeFixtureDef.friction = 0.3f;
        edgeFixtureDef.restitution = 0.3f;
        groundBody->CreateFixture(&edgeFixtureDef);
    }
}

// Generate curve vertices
void PhysicsManager::generateCurveVertices(int numVertices, float startX, float endX) {
    curveVertices.resize(numVertices);
    float step = (endX - startX) / (numVertices - 1);

    for (int i = 0; i < numVertices; ++i) {
        float x = startX + i * step;
        float t = (float)i / (numVertices - 1); 
        float y = (WINDOW_HEIGHT / SCALE / 2.0f) + 5.0f * sinf(t * 2.0f * b2_pi) * cosf(t * b2_pi);
        curveVertices[i] = b2Vec2(x, y); 
    }
}

// Apply gravity
void PhysicsManager::applyGravity(const b2Vec2& gravity) {
    world.SetGravity(gravity);
    for (b2Body* body = world.GetBodyList(); body; body = body->GetNext()) {
        body->SetAwake(true);
    }
}

// Conditionally apply gravity
void PhysicsManager::applyGravityIfNeeded(bool& gravityApplied, float elapsedTime, float triggerTime) {
    if (!gravityApplied && elapsedTime > triggerTime) {
        applyGravity(b2Vec2(0.0f, -0.05f));
        gravityApplied = true;
    }
}

// Step the simulation
void PhysicsManager::step() {
    world.Step(timeStep, velocityIterations, positionIterations);
}

// Render the ground
void PhysicsManager::renderGround(sf::RenderWindow& window) {
    const int numVertices = curveVertices.size();
    sf::VertexArray groundShape(sf::LineStrip, numVertices);

    float offsetX = WINDOW_WIDTH / 2.0f; // Center X in pixels
    float offsetY = WINDOW_HEIGHT;      // Bottom Y in pixels

    for (int i = 0; i < numVertices; ++i) {
        groundShape[i].position = sf::Vector2f(
            curveVertices[i].x * SCALE + offsetX,
            offsetY - (curveVertices[i].y * SCALE)
        );
        groundShape[i].color = sf::Color::Green;
    }

    window.draw(groundShape);
}

// Get the Box2D world
b2World& PhysicsManager::getWorld() {
    return world;
}
