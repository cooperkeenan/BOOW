// PhysicsManager.h
#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>

// Contact Listener for Collision Detection
class ContactListener : public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override {
        std::cout << "Collision detected!" << std::endl;
    }
};

class PhysicsManager {
public:
    PhysicsManager();
    b2World& getWorld();                     // Get reference to the Box2D world
    void step();                             // Update the Box2D simulation
    void renderGround(sf::RenderWindow& window); // Render the ground
    void applyGravity(const b2Vec2& gravity);

private:
    b2World world;                           // Box2D physics world
    float timeStep;                          // Time step for physics simulation
    int32 velocityIterations;               // Velocity solver iterations
    int32 positionIterations;               // Position solver iterations
    ContactListener contactListener;        // Contact listener for collision detection

    b2Body* groundBody;
};

#endif // PHYSICSMANAGER_H
