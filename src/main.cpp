#include <SFML/Graphics.hpp>
#include "Boat.h"
#include "PhysicsManager.h"
#include "Constants.h"
#include <SFML/System/Clock.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boat Out of Water");
    sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

    // Initialize physics manager
    PhysicsManager physicsManager;

    // Create boat at (150, 100) instead of (WINDOW_WIDTH / 2.0f, 100)
    Boat boat(physicsManager.getWorld(), physicsManager, sf::Vector2f(150.0f, 100.0f), sf::Vector2f(40.0f, 20.0f));

    // Immediately adjust the camera to the boat's current position
    {
        b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
        sf::Vector2f initialCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
        view.setCenter(initialCenter);
        window.setView(view);
    }

    sf::Clock clock;
    bool gravityApplied = false;

    // Smoothing factor for camera
    float lerpFactor = 2.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Controls
        float directionX = 0.0f, directionY = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) directionY = 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) directionX = -0.2f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) directionX = 0.2f;

        boat.move(directionX, directionY, 1.0f);

        // Apply gravity after 2 seconds
        physicsManager.applyGravityIfNeeded(gravityApplied, clock.getElapsedTime().asSeconds(), 2.0f);

        // Update physics
        physicsManager.step();
        boat.update();

        // Check if boat needs respawn AFTER physics updates and BEFORE rendering
        if (boat.checkRespawnNeeded()) {
            boat.respawnBoat();
            // After respawn, reset camera immediately to avoid flicker
            b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
            sf::Vector2f instantCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
            view.setCenter(instantCenter);
            window.setView(view);
        } else {
            // Smooth camera follow if not respawning
            b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
            sf::Vector2f targetCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, view.getCenter().y);

            sf::Vector2f currentCenter = view.getCenter();
            sf::Vector2f newCenter = currentCenter + lerpFactor * (targetCenter - currentCenter);
            view.setCenter(newCenter);
            window.setView(view);
        }

        window.clear();
        physicsManager.renderGround(window);
        boat.render(window);
        window.display();
    }

    return 0;
}
