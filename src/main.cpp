#include <SFML/Graphics.hpp>
#include "Boat.h"
#include "PhysicsManager.h"
#include "Constants.h"
#include <SFML/System/Clock.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boat Out of Water");

    // Initialize physics manager
    PhysicsManager physicsManager;

    // Create boat
    Boat boat(physicsManager.getWorld(), sf::Vector2f(WINDOW_WIDTH / 2.0f, 100.0f), sf::Vector2f(40.0f, 20.0f));

    // Timer to track elapsed time
    sf::Clock clock;
    bool gravityApplied = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Check elapsed time and apply gravity after 5 seconds
        if (!gravityApplied) {
            float elapsed = clock.getElapsedTime().asSeconds();
            std::cout << "Elapsed time: " << elapsed << " seconds" << std::endl;
            if (elapsed > 5.0f) {
                std::cout << "Applying gravity..." << std::endl;
                physicsManager.applyGravity(b2Vec2(0.0f, -9.8f)); // Apply gravity downward
                gravityApplied = true;
            }
        }

        // Update physics
        physicsManager.step();

        // Update boat
        boat.update();

        // Render
        window.clear();
        physicsManager.renderGround(window); // Draw the ground
        boat.render(window);                // Draw the boat
        window.display();
    }

    return 0;
}
