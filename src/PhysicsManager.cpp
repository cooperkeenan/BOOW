#include "PhysicsManager.h"
#include "levels.h"
#include "Constants.h"
#include "BuildObstacles.h"
#include <cmath>
#include <iostream>


PhysicsManager::PhysicsManager()
    : world(b2Vec2(0.0f, 0.0f)),
      timeStep(1.0f / 60.0f),
      velocityIterations(8),
      positionIterations(3),
      groundBody(nullptr),
      gravityApplied(false)
{
    // Define the ground body as static
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(0.0f, 0.0f);
    groundBody = world.CreateBody(&groundBodyDef);

    // Initialize obstacles once
    initializeObstacles(level_1());
}


void PhysicsManager::initializeObstacles(const std::vector<Obstacle>& selected_level) {
    auto builtLevel = obstacleManager.buildLevel(selected_level);

    for (const auto& obs : builtLevel) {
        obstacleManager.addObstacle(obs);
    }

    const auto& obsList = obstacleManager.getObstacles();
    for (const auto& obs : obsList) {
        createFixturesFromObstacle(obs, 0.0f, 0.0f);
    }
}


void PhysicsManager::createFixturesFromObstacle(const Obstacle& obs, float offsetX, float offsetY) {
    if (obs.vertices.size() < 2) return; // Need at least 2 vertices to form edges

    for (size_t i = 0; i < obs.vertices.size() - 1; ++i) {
        b2EdgeShape edge;
        b2Vec2 v1(obs.vertices[i].x + offsetX, obs.vertices[i].y + offsetY);
        b2Vec2 v2(obs.vertices[i+1].x + offsetX, obs.vertices[i+1].y + offsetY);

        edge.SetTwoSided(v1, v2);

        b2FixtureDef edgeFixtureDef;
        edgeFixtureDef.shape = &edge;
        edgeFixtureDef.density = 0.0f;
        edgeFixtureDef.friction = 0.3f;
        edgeFixtureDef.restitution = 0.3f;
        edgeFixtureDef.isSensor = obs.isGap;  // Make gap obstacles sensors

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
    const auto& obsList = obstacleManager.getObstacles();

    float offsetX = WINDOW_WIDTH / 2.0f;
    float offsetY = WINDOW_HEIGHT;

    // Draw each non-gap obstacle
    for (const auto& obs : obsList) {
        if (obs.isGap) {
            continue; // Skip rendering gaps
        }

        if (obs.vertices.size() < 2) {
            continue;
        }

        sf::VertexArray groundShape(sf::LineStrip, static_cast<unsigned int>(obs.vertices.size()));

        for (size_t i = 0; i < obs.vertices.size(); ++i) {
            float worldX = obs.vertices[i].x;
            float worldY = obs.vertices[i].y;

            groundShape[static_cast<unsigned int>(i)].position = sf::Vector2f(
                worldX * SCALE + offsetX,
                offsetY - (worldY * SCALE)
            );
            groundShape[static_cast<unsigned int>(i)].color = sf::Color::Green;
        }

        window.draw(groundShape);
    }
}
