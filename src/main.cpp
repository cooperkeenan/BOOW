#include <SFML/Graphics.hpp>
#include "Boat.h"
#include "PhysicsManager.h"
#include "GameState.h"
#include "Menu.h"
#include "Pause.h"
#include "Constants.h"
#include <SFML/System/Clock.hpp>
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
    sf::Clock timerClock;
    float timeRemaining = 30.0f;
    bool timerPaused = false;
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

    // Level Complete Text Setup
    sf::Text levelCompleteText;
    levelCompleteText.setFont(font);
    levelCompleteText.setString("Level Complete!");
    levelCompleteText.setCharacterSize(50);
    levelCompleteText.setFillColor(sf::Color::White);
    levelCompleteText.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = levelCompleteText.getLocalBounds();
    levelCompleteText.setOrigin(textRect.width / 2, textRect.height / 2);
    levelCompleteText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            menu.handleEvent(event, currentState);

            if (currentState == GameState::Playing) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (pauseButton.isMouseOver(window)) {
                        currentState = GameState::Paused;
                        timerPaused = true;
                    }
                }
            } else if (currentState == GameState::Paused) {
                pauseMenu.handleEvent(event, currentState);
            }

            if (currentState == GameState::LevelComplete) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    currentState = GameState::MainMenu;
                }
            }
        }

        if (previousState != currentState && currentState == GameState::Playing) {
            b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
            sf::Vector2f initialCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
            gameView.setCenter(initialCenter);

            gravityApplied = false;
            clock.restart();
            if (previousState != GameState::Paused) {
                timeRemaining = 30.0f;
                timerClock.restart();
                score = 0;
            }
        }

        previousState = currentState;

        window.setView(window.getDefaultView());
        window.clear(sf::Color::Black);

        if (currentState == GameState::MainMenu || currentState == GameState::LevelSelection) {
            window.setView(window.getDefaultView());
            menu.draw(currentState);
        } else if (currentState == GameState::Playing) {
            if (!timerPaused) {
                timeRemaining -= timerClock.restart().asSeconds();
                if (timeRemaining < 0.0f) timeRemaining = 0.0f;
            }

            timerText.setString("Time: " + std::to_string(static_cast<int>(timeRemaining)));
            scoreText.setString("Score: " + std::to_string(score));

            window.setView(gameView);

            float directionX = 0.0f, directionY = 0.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) directionY = 0.5f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) directionX = -0.25f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) directionX = 0.25f;

            boat.move(directionX, directionY, 1.0f);

            physicsManager.applyGravityIfNeeded(gravityApplied, clock.getElapsedTime().asSeconds(), 2.0f);
            physicsManager.step();
            boat.update(currentState);

            if (boat.checkRespawnNeeded()) {
                boat.respawnBoat();
                b2Vec2 boatPos = boat.getBoatBody()->GetPosition();
                sf::Vector2f instantCenter(boatPos.x * SCALE + WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
                gameView.setCenter(instantCenter);
                window.setView(gameView);

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
                window.setView(gameView);
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
        } else if (currentState == GameState::LevelComplete) {
            window.clear(sf::Color::Black);
            window.draw(levelCompleteText);
        }

        window.display();
    }

    return 0;
}
