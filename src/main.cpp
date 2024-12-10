#include <SFML/Graphics.hpp>
#include "Boat.h"
#include "PhysicsManager.h"
#include "GameState.h"
#include "Menu.h"
#include "Pause.h"
#include "Constants.h"
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

    // Timer setup
    sf::Clock timerClock; // Clock to track elapsed time
    float timeRemaining = 30.0f; // Timer starts at 30 seconds
    bool timerPaused = false; // Track if the timer is paused
    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(20);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(10.0f, 10.0f);

    // Define a separate view for the game
    sf::View gameView(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

    // Initialize Pause and Resume Buttons
    Button pauseButton("Pause", {100, 40}, 20, sf::Color::Yellow, sf::Color::Black);
    pauseButton.setFont(font);
    pauseButton.setPosition({WINDOW_WIDTH - 120, 20});  // Top-right corner with some padding

    // Initialize the Pause class
    Pause pauseMenu(window, font);

    // Controls screen setup
    std::vector<std::string> controlsText = {
        "Use left and right arrow keys to rotate",
        "Use up and down arrow keys to control speed",
        "Use escape to pause"
    };
    Button backButton("Back", {200, 50}, 20, sf::Color::Blue, sf::Color::White);
    backButton.setFont(font);
    backButton.setPosition({300, 400});

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
                        timerPaused = true; // Pause the timer
                    }
                }
            } else if (currentState == GameState::Paused) {
                pauseMenu.handleEvent(event, currentState);
                if (currentState == GameState::MainMenu) {
                    boat.respawnBoat(); // Reset the boat position
                    timeRemaining = 30.0f; // Reset the timer
                    timerPaused = true; // Pause the timer
                }
            } else if (currentState == GameState::Controls) {
                // Handle Back button on the Controls screen
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (backButton.isMouseOver(window)) {
                        currentState = GameState::MainMenu;
                        boat.respawnBoat(); // Reset the boat position
                        timeRemaining = 30.0f; // Reset the timer
                        timerPaused = true; // Pause the timer
                    }
                }
            }

            // Handle Escape Key for Pausing and Resuming
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                if (currentState == GameState::Playing) {
                    currentState = GameState::Paused;
                    timerPaused = true; // Pause the timer
                } else if (currentState == GameState::Paused) {
                    currentState = GameState::Playing;
                    timerPaused = false; // Resume the timer
                }
            }
        }

        // If we just switched to playing, initialize the camera properly
        if (previousState != currentState && currentState == GameState::Playing) {
            b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
            sf::Vector2f initialCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
            gameView.setCenter(initialCenter);

            gravityApplied = false;
            clock.restart();

            // Ensure the timer starts correctly when transitioning from other states
            if (previousState != GameState::Paused) { 
                timeRemaining = 30.0f; // Reset to 30 seconds
                timerClock.restart(); // Restart the clock for proper countdown
                timerPaused = false;  // Resume the timer
            }

            // Update timer immediately
            timerText.setString("Time: " + std::to_string(static_cast<int>(timeRemaining)));
        }

        previousState = currentState;

        // Clear the window using the default view
        window.setView(window.getDefaultView());
        window.clear(sf::Color::Black);

        if (currentState == GameState::MainMenu || currentState == GameState::LevelSelection) {
            window.setView(window.getDefaultView());
            menu.draw(currentState);
        } else if (currentState == GameState::Playing) {
            // Update timer if not paused
            if (!timerPaused) {
                timeRemaining -= timerClock.restart().asSeconds();
                if (timeRemaining < 0.0f) timeRemaining = 0.0f; // Clamp timer to 0
            }

            timerText.setString("Time: " + std::to_string(static_cast<int>(timeRemaining)));

            window.setView(gameView);

            // Player input and boat movement
            float directionX = 0.0f; // Movement along x-axis
            float torque = 0.0f;     // Rotation

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) directionX = 0.4f;    // Reduced force for forward
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) directionX = -0.4f; // Reduced force for backward
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) torque = 1.0f;      // Reduced torque for left rotation
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) torque = -1.0f;    // Reduced torque for right rotation

            boat.move(directionX, 0.0f, 5.0f); // Apply movement force
            boat.rotate(torque);              // Apply rotational torque

            physicsManager.applyGravityIfNeeded(gravityApplied, clock.getElapsedTime().asSeconds(), 0.5f);
            physicsManager.step();
            boat.update();

            if (boat.checkRespawnNeeded()) {
                boat.respawnBoat();
                b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
                sf::Vector2f instantCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
                gameView.setCenter(instantCenter);
                window.setView(gameView);

                gravityApplied = false;
                clock.restart();

                // Reset timer only on death
                timeRemaining = 30.0f;
                timerClock.restart();
            } else {
                b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
                sf::Vector2f targetCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, gameView.getCenter().y);
                sf::Vector2f currentCenter = gameView.getCenter();
                sf::Vector2f newCenter = currentCenter + lerpFactor * (targetCenter - currentCenter);
                gameView.setCenter(newCenter);
                window.setView(gameView);
            }

            physicsManager.renderGround(window);
            boat.render(window);

            window.setView(window.getDefaultView());
            window.draw(timerText);
            pauseButton.draw(window);
        } else if (currentState == GameState::Paused) {
            window.setView(gameView);
            physicsManager.renderGround(window);
            boat.render(window);
            window.setView(window.getDefaultView());
            pauseMenu.draw();
        } else if (currentState == GameState::Controls) {
            // Draw controls instructions
            float yOffset = 100.0f;
            for (const auto& line : controlsText) {
                sf::Text text(line, font, 20);
                text.setFillColor(sf::Color::White);
                text.setPosition(50, yOffset);
                yOffset += 50;
                window.draw(text);
            }

            // Draw Back button
            backButton.draw(window);
        }

        window.display();
    }

    return 0;
}