#include "AIController.h"
#include <iostream>



AIController::AIController(Boat& controlledBoat, const std::vector<AIInput>& inputs)
    : boat(controlledBoat), inputs(inputs), currentIndex(0), actionTimer(0.0f) {
}


void AIController::update(float deltaTime) {
    if (currentIndex >= inputs.size()) {
        return;
    }

    const AIInput& currentInput = inputs[currentIndex];
    actionTimer += deltaTime;


    if (currentInput.key != sf::Keyboard::Unknown) {
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

    if (actionTimer >= currentInput.duration) {
        actionTimer = 0.0f;
        currentIndex++;
    }
}


void AIController::reset() {
    currentIndex = 0;   // Reset to the first action
    actionTimer = 0.0f; // Reset the timer
}