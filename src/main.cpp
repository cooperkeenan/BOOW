#include <SFML/Graphics.hpp>
#include "Boat.h"
#include "PhysicsManager.h"

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boat Out of Water");

    // Initialize physics manager
    PhysicsManager physicsManager;

    // Create boat
    Boat boat(physicsManager.getWorld(), sf::Vector2f(400.0f, 300.0f), sf::Vector2f(40.0f, 20.0f));

    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update physics
        physicsManager.step();

        // Update boat
        boat.update();

        // Render
        window.clear();
        boat.render(window);
        window.display();
    }

    return 0;
}
