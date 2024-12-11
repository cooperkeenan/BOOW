#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Boat.h"

struct AIInput {
    sf::Keyboard::Key key; // The key to simulate (e.g., Up, Down, Left, Right)
    float duration;        // How long to perform the action (in seconds)
};

class AIController {
public:
    AIController(Boat& controlledBoat, const std::vector<AIInput>& inputs);

    void update(float deltaTime);

private:
    Boat& boat;                  // Reference to the controlled boat
    std::vector<AIInput> inputs; // List of actions
    size_t currentIndex;         // Current action index
    float actionTimer;           // Timer for the current action
};

#endif // AICONTROLLER_H