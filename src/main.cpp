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

    // Load Font
    sf::Font font;
    if (!font.loadFromFile("../img/RobotoMono-Regular.ttf")) {
        std::cerr << "Failed to load font\n";
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

    // Score setup
    int score = 0;
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.0f, 40.0f);

    // Define a separate view for the game
    sf::View gameView(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

    // Initialize Pause and Resume Buttons
    Button pauseButton("Pause", {100, 40}, 20, sf::Color::Yellow, sf::Color::Black);
    pauseButton.setFont(font);
    pauseButton.setPosition({WINDOW_WIDTH - 120, 20});

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

            // Handle menu events
            menu.handleEvent(event, currentState);

            if (currentState == GameState::Playing) {
                // Handle Pause button
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (pauseButton.isMouseOver(window)) {
                        currentState = GameState::Paused;
                        timerPaused = true; // Pause the timer
                    }
                }
            } else if (currentState == GameState::Paused) {
                pauseMenu.handleEvent(event, currentState);
                if (currentState == GameState::MainMenu) {
                    boat.respawnBoat();
                    timeRemaining = 30.0f; // Reset timer
                    timerPaused = true;
                }
            } else if (currentState == GameState::Controls) {
                // Handle Back button on Controls screen
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (backButton.isMouseOver(window)) {
                        currentState = GameState::MainMenu;
                        boat.respawnBoat();
                        timeRemaining = 30.0f;
                        timerPaused = true;
                    }
                }
            }

            // Handle Escape Key
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                if (currentState == GameState::Playing) {
                    currentState = GameState::Paused;
                    timerPaused = true;
                } else if (currentState == GameState::Paused) {
                    currentState = GameState::Playing;
                    timerPaused = false;
                }
            }
        }

        // Initialize camera and timer when switching to Playing
        if (previousState != currentState && currentState == GameState::Playing) {
            b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
            sf::Vector2f initialCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
            gameView.setCenter(initialCenter);

            gravityApplied = false;
            clock.restart();

            if (previousState != GameState::Paused) {
                timeRemaining = 30.0f;
                timerClock.restart();
                timerPaused = false;
            }

            timerText.setString("Time: " + std::to_string(static_cast<int>(timeRemaining)));
        }

        previousState = currentState;

        window.setView(window.getDefaultView());
        window.clear(sf::Color::Black);

        if (currentState == GameState::MainMenu || currentState == GameState::LevelSelection) {
            menu.draw(currentState);
        } else if (currentState == GameState::Playing) {
            if (!timerPaused) {
                timeRemaining -= timerClock.restart().asSeconds();
                if (timeRemaining < 0.0f) timeRemaining = 0.0f;
            }

            timerText.setString("Time: " + std::to_string(static_cast<int>(timeRemaining)));
            scoreText.setString("Score: " + std::to_string(score));

            window.setView(gameView);

            // Handle player input
            float directionX = 0.0f;
            float torque = 0.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) directionX = 0.4f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) directionX = -0.4f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) torque = 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) torque = -1.0f;

            boat.move(directionX, 0.0f, 5.0f);
            boat.rotate(torque);

            physicsManager.applyGravityIfNeeded(gravityApplied, clock.getElapsedTime().asSeconds(), 0.5f);
            physicsManager.step();
            boat.update(currentState);

            // Respawn boat if needed
            if (boat.checkRespawnNeeded()) {
                boat.respawnBoat();
                gravityApplied = false;
                clock.restart();
                timeRemaining = 30.0f;
                timerClock.restart();
                score = 0;
            } else {
                b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
                sf::Vector2f targetCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, gameView.getCenter().y);
                sf::Vector2f currentCenter = gameView.getCenter();
                sf::Vector2f newCenter = currentCenter + lerpFactor * (targetCenter - currentCenter);
                gameView.setCenter(newCenter);
            }

            score += physicsManager.checkCollectables();

            physicsManager.renderGround(window);
            physicsManager.renderCollectables(window);
            boat.render(window);

            window.setView(window.getDefaultView());
            window.draw(timerText);
            window.draw(scoreText);
            pauseButton.draw(window);

        } else if (currentState == GameState::Paused) {
            window.setView(gameView);
            physicsManager.renderGround(window);
            physicsManager.renderCollectables(window);
            boat.render(window);
            window.setView(window.getDefaultView());
            pauseMenu.draw();
        } else if (currentState == GameState::Controls) {
            float yOffset = 100.0f;
            for (const auto& line : controlsText) {
                sf::Text text(line, font, 20);
                text.setFillColor(sf::Color::White);
                text.setPosition(50, yOffset);
                yOffset += 50;
                window.draw(text);
            }

            backButton.draw(window);
        } else if (currentState == GameState::LevelComplete) {
            menu.drawLevelCompleteScreen(window);
        }

        window.display();
    }

    return 0;
}
