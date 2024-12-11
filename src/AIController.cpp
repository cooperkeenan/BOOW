#include "AIController.h"
#include <iostream>



AIController::AIController(Boat& controlledBoat, const std::vector<AIInput>& inputs)
    : boat(controlledBoat), inputs(inputs), currentIndex(0), actionTimer(0.0f) {
    std::cout << "AIController constructor called." << std::endl;
}


void AIController::update(float deltaTime) {
    if (currentIndex >= inputs.size()) {
        std::cout << "AIController: No more actions to perform.\n";
        return;
    }

    const AIInput& currentInput = inputs[currentIndex];
    actionTimer += deltaTime;

    std::cout << "AIController: Performing action " << currentIndex 
              << " for " << currentInput.duration 
              << " seconds. Timer: " << actionTimer << "\n";

    if (currentInput.key != sf::Keyboard::Unknown) {
        if (currentInput.key == sf::Keyboard::Up) {
            boat.move(0.4f, 0.0f, 5.0f);
            std::cout << "AIController: Moving forward.\n";
        } else if (currentInput.key == sf::Keyboard::Down) {
            boat.move(-0.4f, 0.0f, 5.0f);
            std::cout << "AIController: Moving backward.\n";
        } else if (currentInput.key == sf::Keyboard::Left) {
            boat.rotate(1.0f);
            std::cout << "AIController: Turning left.\n";
        } else if (currentInput.key == sf::Keyboard::Right) {
            boat.rotate(-1.0f);
            std::cout << "AIController: Turning right.\n";
        }
    }

    if (actionTimer >= currentInput.duration) {
        actionTimer = 0.0f;
        currentIndex++;
        std::cout << "AIController: Moving to next action.\n";
    }
}


void AIController::reset() {
    currentIndex = 0;   // Reset to the first action
    actionTimer = 0.0f; // Reset the timer
}