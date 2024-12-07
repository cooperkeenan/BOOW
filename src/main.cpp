//Main.cpp


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

    sf::Clock clock;
    bool gravityApplied = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Controls
        float directionX = 0.0f, directionY = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) directionY = 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) directionX = -0.4f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) directionX = 0.4f;

        boat.move(directionX, directionY, 1.0f);

        // Apply gravity after 2 seconds
        physicsManager.applyGravityIfNeeded(gravityApplied, clock.getElapsedTime().asSeconds(), 2.0f);

        // Update physics and render
        physicsManager.step();
        boat.update();

        window.clear();
        physicsManager.renderGround(window);
        boat.render(window);
        window.display();
    }

    return 0;
}
