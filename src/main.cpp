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

            // Delegate event handling to the new function
            handleGameEvents(window, event, components);
        }

        // Render the game state
        renderGameState(components, window);
    }

    return 0;
}