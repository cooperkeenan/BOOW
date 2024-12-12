#include <SFML/Graphics.hpp>
#include "GameSetup.h"
#include "RenderGame.h"
#include "EventHandler.h"

int main() {
    // Create the main game window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boat Out of Water");

    // Initialize all game components
    GameComponents components = initializeGame(window);

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;

        // Handle window events (e.g., closing the window)
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); // Close the window if the close event is triggered
            }

            // Handle game-specific events based on current game state
            handleGameEvents(window, event, components);
        }

        // Update game state (physics, AI, etc.) if the game is in the Playing state
        if (components.currentState == GameState::Playing) {
            // Get the elapsed time since the last update
            float elapsedTime = components.clock.restart().asSeconds();

            // Apply gravity to the player's boat if needed
            components.physicsManager->applyGravityIfNeeded(
                components.gravityApplied, components.gravityClock.getElapsedTime().asSeconds(), 0.5f);

            // Update the AI controller for the AI boat
            components.aiController->update(elapsedTime);

            // Step the Box2D physics simulation
            components.physicsManager->step();
        }

        // Render the game based on the current game state
        renderGameState(components, window);
    }

    return 0; // Successful exit
}