#include "PhysicsManager.h"
#include "Constants.h"
#include "BuildObstacles.h"

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

    // Generate the track vertices
    obstacleManager.generateTrack(trackVertices, 2); // Generate a track with 2 obstacles

    // Create Box2D fixtures from the generated track
    createFixturesFromTrack();
}

void PhysicsManager::initializeObstacles() {
    // Add predefined obstacles (line, ramp, gap, line)
    auto obstacles = createSimpleRampSet();
    for (const auto& obs : obstacles) {
        obstacleManager.addObstacle(obs);
    }
}

void PhysicsManager::createFixturesFromTrack() {
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

b2World& PhysicsManager::getWorld() {
    return world;
}

void PhysicsManager::applyGravity(const b2Vec2& gravity) {
    world.SetGravity(gravity);
    for (b2Body* body = world.GetBodyList(); body; body = body->GetNext()) {
        body->SetAwake(true);
    }
}

void PhysicsManager::applyGravityIfNeeded(bool& gravityApplied, float elapsedTime, float triggerTime) {
    if (!gravityApplied && elapsedTime > triggerTime) {
        applyGravity(b2Vec2(0.0f, -0.05f));
        gravityApplied = true;
    }
}

void PhysicsManager::step() {
    world.Step(timeStep, velocityIterations, positionIterations);
}

void PhysicsManager::renderGround(sf::RenderWindow& window) {
    const int numVertices = static_cast<int>(trackVertices.size());
    sf::VertexArray groundShape(sf::LineStrip, numVertices);

    float offsetX = WINDOW_WIDTH / 2.0f; // Center X in pixels
    float offsetY = WINDOW_HEIGHT;       // Bottom Y in pixels

    for (int i = 0; i < numVertices; ++i) {
        groundShape[i].position = sf::Vector2f(
            trackVertices[i].x * SCALE + offsetX,
            offsetY - (trackVertices[i].y * SCALE)
        );
        groundShape[i].color = sf::Color::Green;
    }

    window.draw(groundShape);
}
