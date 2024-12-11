#include <SFML/Graphics.hpp>
#include "GameSetup.h"
#include "RenderGame.h"
#include "EventHandler.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boat Out of Water");

    // Initialize game components
    GameComponents components = initializeGame(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle game events
            handleGameEvents(window, event, components);
        }

        // Update game state (e.g., physics and logic)
        if (components.currentState == GameState::Playing) {
            float elapsedTime = components.clock.restart().asSeconds();

            // Apply gravity for player and AI boats
            components.physicsManager->applyGravityIfNeeded(
                components.gravityApplied, components.gravityClock.getElapsedTime().asSeconds(), 0.5f);
            
            // Update AI
            components.aiController->update(elapsedTime);

            // Step the physics simulation
            components.physicsManager->step();
        }

        // Render the game
        renderGameState(components, window);
    }
}