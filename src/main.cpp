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

    // Create boat 
    Boat boat(physicsManager.getWorld(), physicsManager, sf::Vector2f(150.0f, 100.0f), sf::Vector2f(40.0f, 20.0f));

    // Load Font
    sf::Font font;
    if (!font.loadFromFile("img/RobotoMono-Regular.ttf")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    // Create score text
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.0f, 10.0f);

    int score = 0;

    // Adjust the camera to boat's position
    {
        b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
        sf::Vector2f initialCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
        view.setCenter(initialCenter);
        window.setView(view);
    }



    sf::Clock clock;
    bool gravityApplied = false;
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) directionX = -0.25f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) directionX = 0.25f;
        boat.move(directionX, directionY, 1.0f);

        // Apply gravity after 2 seconds
        physicsManager.applyGravityIfNeeded(gravityApplied, clock.getElapsedTime().asSeconds(), 2.0f);

        // Update physics
        physicsManager.step();
        boat.update();

        // Check if boat needs respawn
        if (boat.checkRespawnNeeded()) {
            boat.respawnBoat();
            b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
            sf::Vector2f instantCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
            view.setCenter(instantCenter);
            window.setView(view);
        } else {
            b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
            sf::Vector2f targetCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, view.getCenter().y);
            sf::Vector2f currentCenter = view.getCenter();
            sf::Vector2f newCenter = currentCenter + lerpFactor * (targetCenter - currentCenter);
            view.setCenter(newCenter);
            window.setView(view);
        }

        // Update score
        score += physicsManager.checkCollectables();
        scoreText.setString("Score: " + std::to_string(score));

        // Clear window and render scene
        window.clear();
        physicsManager.renderGround(window);
        physicsManager.renderCollectables(window);
        boat.render(window);

        // Draw score in the fixed position
        window.setView(window.getDefaultView());
        window.draw(scoreText);
        window.setView(view);

        
        window.display();
    }

    return 0;
}