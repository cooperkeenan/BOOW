#include "PhysicsManager.h"
#include "levels.h"
#include "Constants.h"
#include "BuildObstacles.h"
#include <cmath>
#include <iostream>

// Constructor: Initializes the Box2D world, time step, iterations, ground body, and other physics parameters.
PhysicsManager::PhysicsManager()
    : world(b2Vec2(0.0f, 0.0f)), // Initialize the Box2D world with no gravity initially.
      timeStep(1.0f / 60.0f),   // Set the time step for the physics simulation.
      velocityIterations(8),     // Set the number of velocity iterations per step.
      positionIterations(3),     // Set the number of position iterations per step.
      groundBody(nullptr),       // Initialize the ground body pointer to null.
      gravityApplied(false)      // Initialize gravityApplied to false.
{
    // Define the ground body as static.
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody; // Static body does not move.
    groundBodyDef.position.Set(0.0f, 0.0f); // Set the ground body's initial position.
    groundBody = world.CreateBody(&groundBodyDef); // Create the ground body in the Box2D world.

    // Select Level
    initializeObstacles(level_1()); // Initialize obstacles for level 1.

    // Initialize collectables
    collectables.emplace_back(world, sf::Vector2f(500, 280), 10.0f); // create collectable
    collectables.emplace_back(world, sf::Vector2f(800, 280), 10.0f); // create collectable
    collectables.emplace_back(world, sf::Vector2f(1050, 280), 10.0f); // create collectable

}

// Adds each obstacle from the selected level to the obstacle manager and creates corresponding Box2D fixtures.
void PhysicsManager::initializeObstacles(const std::vector<Obstacle>& selected_level) {
    for (const auto& obs : selected_level) {
        obstacleManager.addObstacle(obs); // Add the obstacle to the manager.
        createFixturesFromObstacle(obs);   // Create Box2D fixtures for the obstacle.
    }
}

// Creates Box2D edge fixtures for a given obstacle, including collision filtering.
void PhysicsManager::createFixturesFromObstacle(const Obstacle& obs) {
    if (obs.vertices.size() < 2) return; // If obstacle has less than 2 vertices, it cant be an edge

    for (size_t i = 0; i < obs.vertices.size() - 1; ++i) {
        b2EdgeShape edge;
        b2Vec2 v1(obs.vertices[i].x, obs.vertices[i].y);     // Get the start vertex of the edge.
        b2Vec2 v2(obs.vertices[i + 1].x, obs.vertices[i + 1].y); // Get the end vertex of the edge.

        edge.SetTwoSided(v1, v2); // Set the edge shape using the two vertices.

        b2FixtureDef edgeFixtureDef;
        edgeFixtureDef.shape = &edge;         // Set the shape of the fixture.
        edgeFixtureDef.density = 0.0f;       // Set the density of the fixture (0 for static objects).
        edgeFixtureDef.friction = 0.3f;      // Set the friction of the fixture.
        edgeFixtureDef.restitution = 0.3f;   // Set the restitution (bounciness) of the fixture.
        edgeFixtureDef.isSensor = obs.isGap; //check if it is a gap

        // Collision filtering for obstacles
        edgeFixtureDef.filter.categoryBits = CATEGORY_OBSTACLE; // Set the category bits for obstacles.
        edgeFixtureDef.filter.maskBits = CATEGORY_PLAYER | CATEGORY_AI; // Set the mask bits to collide with player and AI boats.

        groundBody->CreateFixture(&edgeFixtureDef); // Create the fixture on the ground body.
    }
}

// Returns a reference to the Box2D world.
b2World& PhysicsManager::getWorld() {
    return world; // Return a reference to the Box2D world.
}

// Applies gravity to the Box2D world and wakes up all bodies.
void PhysicsManager::applyGravity(const b2Vec2& gravity) {
    world.SetGravity(gravity); // Set the gravity vector for the world.
    for (b2Body* body = world.GetBodyList(); body; body = body->GetNext()) {
        body->SetAwake(true); // Wake up all bodies in the world to apply the new gravity.
    }
}

// Applies gravity only if it hasn't been applied yet and a certain time has passed.
void PhysicsManager::applyGravityIfNeeded(bool& gravityApplied, float elapsedTime, float triggerTime) {
    if (!gravityApplied && elapsedTime > triggerTime) {
        applyGravity(b2Vec2(0.0f, -0.02f)); // Apply downwards gravity.
        gravityApplied = true;            // Set the gravityApplied flag to true.
    }
}

// Steps the Box2D physics simulation forward by one time step.
void PhysicsManager::step() {
    world.Step(timeStep, velocityIterations, positionIterations); // Perform a physics step.
}

// Renders the ground (obstacles) to the SFML window.
void PhysicsManager::renderGround(sf::RenderWindow& window) {
    const auto& obsList = obstacleManager.getObstacles(); // Get the list of obstacles.

    float offsetX = WINDOW_WIDTH / 2.0f;  // Offset to center the ground horizontally.
    float offsetY = WINDOW_HEIGHT; // Offset from the bottom of the window.

    for (const auto& obs : obsList) {
        if (obs.isGap) {
            continue; // Skip drawing gaps.
        }

        sf::VertexArray groundShape(sf::LineStrip, static_cast<unsigned int>(obs.vertices.size())); // Create a vertex array for the ground shape.

        for (size_t i = 0; i < obs.vertices.size(); ++i) {
            float worldX = obs.vertices[i].x; // Get the x-coordinate in world units.
            float worldY = obs.vertices[i].y; // Get the y-coordinate in world units.

            groundShape[static_cast<unsigned int>(i)].position = sf::Vector2f(
                worldX * SCALE + offsetX,    // Convert world x to screen x and apply offset.
                offsetY - (worldY * SCALE) // Convert world y to screen y and apply offset.
            );
            groundShape[static_cast<unsigned int>(i)].color = sf::Color::Green; // Set the color of the ground.
        }

        window.draw(groundShape); // Draw the ground shape.
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

// Resets the physics world by destroying existing fixtures and recreating them with new level data.
void PhysicsManager::reset(const std::vector<Obstacle>& selected_level) {
    // Destroy all existing fixtures attached to the ground body.
    b2Fixture* f = groundBody->GetFixtureList();
    while (f) {
        b2Fixture* next = f->GetNext(); // Get the next fixture before destroying the current one.
        groundBody->DestroyFixture(f);   // Destroy the current fixture.
        f = next;                       // Move to the next fixture.
    }

    obstacleManager.clearObstacles(); // Clear the obstacle manager.
    collectables.clear(); // Clear collect