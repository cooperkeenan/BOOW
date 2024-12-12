#include "AIController.h"
#include <iostream>

// Constructor: Initializes the AI controller with a controlled boat and a sequence of AI inputs.
AIController::AIController(Boat& controlledBoat, const std::vector<AIInput>& inputs)
    : boat(controlledBoat), inputs(inputs), currentIndex(0), actionTimer(0.0f) {
}

// Updates the AI controller. Simulates key presses based on the input sequence.
void AIController::update(float deltaTime) {
    // If all inputs have been processed exit the function
    if (currentIndex >= inputs.size()) {
        return;
    }

    const AIInput& currentInput = inputs[currentIndex]; // Get the current input action.
    actionTimer += deltaTime;                          // Increment the action timer.

    // Simulate key press if a key is specified
    if (currentInput.key != sf::Keyboard::Unknown) {
        if (currentInput.key == sf::Keyboard::Up) {
            boat.move(0.4f, 0.0f, 5.0f); // Move the boat forward.
        } else if (currentInput.key == sf::Keyboard::Down) {
            boat.move(-0.4f, 0.0f, 5.0f); // Move the boat backward.
        } else if (currentInput.key == sf::Keyboard::Left) {
            boat.rotate(1.0f);           // Rotate the boat counter-clockwise.
        } else if (currentInput.key == sf::Keyboard::Right) {
            boat.rotate(-1.0f);           // Rotate the boat clockwise.
        }
    }

    // If the current action's duration has elapsed move to the next action.
    if (actionTimer >= currentInput.duration) {
        actionTimer = 0.0f; // Reset the timer for the next action.
        currentIndex++;     // Move to the next input in the sequence.
    }
}

// Resets the AI controller to the beginning of the input sequence.
void AIController::reset() {
    currentIndex = 0;    // Reset to the first action.
    actionTimer = 0.0f; // Reset the timer.
}