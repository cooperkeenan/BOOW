#include <SFML/Graphics.hpp>
#include "Boat.h"
#include "PhysicsManager.h"
#include "GameState.h"
#include "Menu.h"
#include "Pause.h"          // Include the Pause class
#include "Constants.h"
#include <SFML/System/Clock.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boat Out of Water");


    sf::Font font;
    if (!font.loadFromFile("../../arial.ttf")) {  // Adjust this path if needed
        std::cerr << "Failed to load font" << std::endl;
        return -1;
    }

    // Initialize menu and game components
    Menu menu(window, font);
    PhysicsManager physicsManager;
    Boat boat(physicsManager.getWorld(), physicsManager, sf::Vector2f(150.0f, 100.0f), sf::Vector2f(40.0f, 20.0f));
    sf::Clock clock;
    bool gravityApplied = false;
    float lerpFactor = 2.0f;
    GameState currentState = GameState::MainMenu;
    GameState previousState = currentState;

    // Define a separate view for the game
    sf::View gameView(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

    // Initialize Pause and Resume Buttons
    Button pauseButton("Pause", {100, 40}, 20, sf::Color::Yellow, sf::Color::Black);
    pauseButton.setFont(font);
    pauseButton.setPosition({WINDOW_WIDTH - 120, 20});  // Top-right corner with some padding

    // Initialize the Pause class
    Pause pauseMenu(window, font);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle menu events, possibly switching states
            menu.handleEvent(event, currentState);

            // Handle Pause and Resume button clicks
            if (currentState == GameState::Playing) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (pauseButton.isMouseOver(window)) {
                        currentState = GameState::Paused;
                        // Restart clock to prevent immediate gravity application after resuming
                        clock.restart();
                    }
                }
            }
            else if (currentState == GameState::Paused) {
                pauseMenu.handleEvent(event, currentState);
            }

            // **Handle Escape Key for Pausing and Resuming**
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                if (currentState == GameState::Playing) {
                    currentState = GameState::Paused;
                    // Restart clock to prevent immediate gravity application after resuming
                    clock.restart();
                }
                else if (currentState == GameState::Paused) {
                    currentState = GameState::Playing;
                    // Restart clock as gameplay resumes
                    clock.restart();
                }
            }
        }

        // If we just switched to playing, initialize the camera properly
        if (previousState != currentState && currentState == GameState::Playing) {
            // Center camera on the boat at the start
            b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
            sf::Vector2f initialCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
            gameView.setCenter(initialCenter);

            gravityApplied = false;
            clock.restart();
        }

        previousState = currentState;

        // Clear the window using the default view
        window.setView(window.getDefaultView());
        window.clear(sf::Color::Black);

        if (currentState == GameState::MainMenu || currentState == GameState::LevelSelection) {
            // Draw UI using the default view
            window.setView(window.getDefaultView());
            menu.draw(currentState);
        }
        else if (currentState == GameState::Playing) {
            // Switch to the game view
            window.setView(gameView);

            // Player input and boat movement
            float directionX = 0.0f, directionY = 0.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    directionY = 0.5f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  directionX = -0.25f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) directionX = 0.25f;

            boat.move(directionX, directionY, 1.0f);

            // Apply gravity and update physics only if not paused
            physicsManager.applyGravityIfNeeded(gravityApplied, clock.getElapsedTime().asSeconds(), 2.0f);
            physicsManager.step();
            boat.update();

            // Check respawn
            if (boat.checkRespawnNeeded()) {
                boat.respawnBoat();
                // Re-center camera
                b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
                sf::Vector2f instantCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
                gameView.setCenter(instantCenter);
                window.setView(gameView);

                gravityApplied = false;
                clock.restart();
            }
            else {
                // Smooth camera follow
                b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
                sf::Vector2f targetCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, gameView.getCenter().y);
                sf::Vector2f currentCenter = gameView.getCenter();
                sf::Vector2f newCenter = currentCenter + lerpFactor * (targetCenter - currentCenter);
                gameView.setCenter(newCenter);
                window.setView(gameView);
            }

            // Render game world
            physicsManager.renderGround(window);
            boat.render(window);

            // Draw Pause Button
            window.setView(window.getDefaultView());  // Draw UI elements like buttons using default view
            pauseButton.draw(window);
        }
        else if (currentState == GameState::Paused) {
            // Draw the game view as is without updating
            window.setView(gameView);
            // Render game world (so the pause overlay appears over it)
            physicsManager.renderGround(window);
            boat.render(window);

            // Draw the pause menu (overlay and Resume button)
            window.setView(window.getDefaultView());  // Draw UI elements using default view
            pauseMenu.draw();
        }

        window.display();
    }

    return 0;
}