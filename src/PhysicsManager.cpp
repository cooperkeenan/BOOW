#include "PhysicsManager.h"
#include "levels.h"
#include "Constants.h"
#include "BuildObstacles.h"
#include <cmath>
#include <iostream>

// Collision categories

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

    // Select Level
    initializeObstacles(level_1());

    // Initialize collectables
    collectables.emplace_back(world, sf::Vector2f(500, 280), 10.0f);
    collectables.emplace_back(world, sf::Vector2f(800, 280), 10.0f);
    collectables.emplace_back(world, sf::Vector2f(1050, 280), 10.0f);

}

//Add each obstacle to the manager  
void PhysicsManager::initializeObstacles(const std::vector<Obstacle>& selected_level) {
    for (const auto& obs : selected_level) {
        obstacleManager.addObstacle(obs);
        createFixturesFromObstacle(obs);
    }
}

// Create Fixtures with Collision Filtering
void PhysicsManager::createFixturesFromObstacle(const Obstacle& obs) {
    if (obs.vertices.size() < 2) return;

    for (size_t i = 0; i < obs.vertices.size() - 1; ++i) {
        b2EdgeShape edge;
        b2Vec2 v1(obs.vertices[i].x, obs.vertices[i].y);
        b2Vec2 v2(obs.vertices[i + 1].x, obs.vertices[i + 1].y);

        edge.SetTwoSided(v1, v2);

        b2FixtureDef edgeFixtureDef;
        edgeFixtureDef.shape = &edge;
        edgeFixtureDef.density = 0.0f;
        edgeFixtureDef.friction = 0.3f;
        edgeFixtureDef.restitution = 0.3f;
        edgeFixtureDef.isSensor = obs.isGap;

        // Collision filtering for obstacles
        edgeFixtureDef.filter.categoryBits = CATEGORY_OBSTACLE; // Set as obstacle
        edgeFixtureDef.filter.maskBits = CATEGORY_PLAYER | CATEGORY_AI; // Collides with both player and AI boats

        groundBody->CreateFixture(&edgeFixtureDef);
    }
}

// Update Physics
b2World& PhysicsManager::getWorld() {
    return world;
}

// Apply gravity
void PhysicsManager::applyGravity(const b2Vec2& gravity) {
    world.SetGravity(gravity);
    for (b2Body* body = world.GetBodyList(); body; body = body->GetNext()) {
        body->SetAwake(true);
    }
}

// Apply gravity after set time
void PhysicsManager::applyGravityIfNeeded(bool& gravityApplied, float elapsedTime, float triggerTime) {
    if (!gravityApplied && elapsedTime > triggerTime) {
        applyGravity(b2Vec2(0.0f, -0.02f));
        gravityApplied = true;
    }
}

// Move world forward one timestep
void PhysicsManager::step() {
    world.Step(timeStep, velocityIterations, positionIterations);
}

// Render Track
void PhysicsManager::renderGround(sf::RenderWindow& window) {
    const auto& obsList = obstacleManager.getObstacles();

    float offsetX = WINDOW_WIDTH / 2.0f;
    float offsetY = WINDOW_HEIGHT;

    for (const auto& obs : obsList) {
        if (obs.isGap) {
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

//Render Collectables
void PhysicsManager::renderCollectables(sf::RenderWindow& window) {
    for (auto& collectable : collectables) {
        collectable.render(window);
    }
}

//Check Collectables
int PhysicsManager::checkCollectables() {
    int collectedCount = 0;
    for (auto& collectable : collectables) {
        if (!collectable.isCollected() && collectable.getBody()->GetContactList() != nullptr) {
            std::cout << "Contact detected for collectable!" << std::endl;
            auto contact = collectable.getBody()->GetContactList();
            while (contact) {
                std::cout << "Contact with category: " << contact->other->GetFixtureList()->GetFilterData().categoryBits << std::endl;
                if (contact->other->GetFixtureList()->GetFilterData().categoryBits == CATEGORY_PLAYER) {
                    collectable.markCollected();
                    collectedCount++;
                    break;
                }
                contact = contact->next;
            }
        }

    }
    return collectedCount;
}





void PhysicsManager::reset(const std::vector<Obstacle>& selected_level) {
    // Destroy all existing fixtures on the ground body.
    b2Fixture* f = groundBody->GetFixtureList();
    while (f) {
        b2Fixture* next = f->GetNext();
        groundBody->DestroyFixture(f);
        f = next;
    }

    // Clear any existing obstacles and collectables
    collectables.clear();

    // Re-initialize obstacles for the chosen level
    initializeObstacles(selected_level);

    // Re-add collectables for the new level
    collectables.emplace_back(world, sf::Vector2f(500, 280), 10.0f);
    collectables.emplace_back(world, sf::Vector2f(800, 280), 10.0f);
    collectables.emplace_back(world, sf::Vector2f(1050, 280), 10.0f);
}

