#include "AIController.h"
#include <iostream>


// The AIController class automates boat movement by simulating key inputs over time.
AIController::AIController(Boat& controlledBoat, const std::vector<AIInput>& inputs)
    : boat(controlledBoat), inputs(inputs), currentIndex(0), actionTimer(0.0f) {
}


// Update the AI controller each frame, applying movements/rotations based on a predefined sequence.
void AIController::update(float deltaTime) {
    if (currentIndex >= inputs.size()) {
        return;
    }

    const AIInput& currentInput = inputs[currentIndex];
    actionTimer += deltaTime;  //timer

    // Execute the current input action if it's valid.
    if (currentInput.key != sf::Keyboard::Unknown) {
        // Simulate key presses on the boat.
        if (currentInput.key == sf::Keyboard::Up) {
            boat.move(0.4f, 0.0f, 5.0f);
        } else if (currentInput.key == sf::Keyboard::Down) {
            boat.move(-0.4f, 0.0f, 5.0f);
        } else if (currentInput.key == sf::Keyboard::Left) {
            boat.rotate(1.0f);
        } else if (currentInput.key == sf::Keyboard::Right) {
            boat.rotate(-1.0f);
        }
    }

    // Once the current action duration has passed, move on to the next action.
    if (actionTimer >= currentInput.duration) {
        actionTimer = 0.0f;
        currentIndex++;
    }
}


// Reset the AI controller to its initial state, starting again from the first action.
void AIController::reset() {
    currentIndex = 0;   // Go back to the first action in the sequence
    actionTimer = 0.0f; // Reset the action timer
}
