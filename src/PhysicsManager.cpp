#include "PhysicsManager.h"
#include "Constants.h"
#include"ObstacleManager.h"
#include <iostream> // For debugging

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

    // Initialize predefined obstacles
    initializeObstacles();

    // Generate the track
    generateTrack();

    // Create Box2D physics boundaries from track vertices
    for (size_t i = 0; i < trackVertices.size() - 1; ++i) {
        b2EdgeShape edge;
        edge.SetTwoSided(trackVertices[i], trackVertices[i + 1]);

        b2FixtureDef edgeFixtureDef;
        edgeFixtureDef.shape = &edge;
        edgeFixtureDef.density = 0.0f;
        edgeFixtureDef.friction = 0.3f;
        edgeFixtureDef.restitution = 0.3f;
        groundBody->CreateFixture(&edgeFixtureDef);
    }
}

// Initialize predefined obstacles
void PhysicsManager::initializeObstacles() {
    obstacleManager.addObstacle({ {{0, 0}, {5, 2}, {10, 4}, {15, 6}}, 0.0f, 6.0f }); // Ramp
    obstacleManager.addObstacle({ {{0, 0}, {5, 2}, {10, 0}}, 0.0f, 0.0f });          // Bump
}

// Generate the track using obstacles
void PhysicsManager::generateTrack() {
    obstacleManager.generateTrack(trackVertices, 10); // Generate track with 10 obstacles
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

// Step the physics simulation
void PhysicsManager::step() {
    world.Step(timeStep, velocityIterations, positionIterations);
}

// Render the ground
void PhysicsManager::renderGround(sf::RenderWindow& window) {
    const int numVertices = trackVertices.size();
    sf::VertexArray groundShape(sf::LineStrip, numVertices);

    float offsetX = WINDOW_WIDTH / 2.0f; // Center X in pixels
    float offsetY = WINDOW_HEIGHT;      // Bottom Y in pixels

    for (int i = 0; i < numVertices; ++i) {
        groundShape[i].position = sf::Vector2f(
            trackVertices[i].x * SCALE + offsetX,
            offsetY - (trackVertices[i].y * SCALE)
        );
        groundShape[i].color = sf::Color::Green;
    }

    window.draw(groundShape);
}

// Access the Box2D world
b2World& PhysicsManager::getWorld() {
    return world;
}
