#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
    : world(b2Vec2(0.0f, 0.0f)), timeStep(1.0f / 60.0f), velocityIterations(8), positionIterations(3) {}

b2World& PhysicsManager::getWorld() {
    return world;
}

void PhysicsManager::step() {
    world.Step(timeStep, velocityIterations, positionIterations);
}
