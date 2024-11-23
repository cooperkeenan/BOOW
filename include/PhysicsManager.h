#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <Box2D/Box2D.h>

class PhysicsManager {
public:
    PhysicsManager();
    b2World& getWorld();
    void step();

private:
    b2World world;
    float timeStep;
    int32 velocityIterations;
    int32 positionIterations;
};

#endif // PHYSICSMANAGER_H
