#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Boat.h"

// Structure to define AI input actions.
struct AIInput {
    sf::Keyboard::Key key; // The key to simulate.
    float duration;         // The duration of the key press in seconds.
};

// Manages AI control for a boat.
class AIController {
public:
    // Constructor initializes the AI controller with a boat and a sequence of inputs.
    AIController(Boat& controlledBoat, const std::vector<AIInput>& inputs);

    // Updates the AI simulating key presses based on the input sequence.
    void update(float deltaTime);

    // Resets the AI to the beginning of its input sequence.
    void reset();

private:
    Boat& boat;                    // Reference to the boat being controlled.
    std::vector<AIInput> inputs; // Sequence of AI input actions.
    size_t currentIndex;         // Index of the current action in the sequence.
    float actionTimer;             // Timer for the current action.
};