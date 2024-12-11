#include "Pause.h"

// Constructor: Initializes the overlay and buttons
Pause::Pause(sf::RenderWindow& window, sf::Font& font)
    : window(window),
      resumeButton("Resume", {200, 50}, 20, sf::Color::Green, sf::Color::Black),
      mainMenuButton("Main Menu", {200, 50}, 20, sf::Color::Red, sf::Color::Black) {
    overlay.setSize(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    overlay.setPosition(0, 0);

    resumeButton.setFont(font);
    resumeButton.setPosition({
        (static_cast<float>(window.getSize().x) - resumeButton.getSize().x) / 2.0f,
        (static_cast<float>(window.getSize().y) - resumeButton.getSize().y) / 2.0f - 60
    });

    mainMenuButton.setFont(font);
    mainMenuButton.setPosition({
        (static_cast<float>(window.getSize().x) - mainMenuButton.getSize().x) / 2.0f,
        (static_cast<float>(window.getSize().y) - mainMenuButton.getSize().y) / 2.0f + 10
    });
}

// Handles events when the game is paused
void Pause::handleEvent(const sf::Event& event, GameState& currentState) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (resumeButton.isMouseOver(window)) {
            currentState = GameState::Playing;  // Resume the game
        } else if (mainMenuButton.isMouseOver(window)) {
            currentState = GameState::MainMenu;  // Return to the Main Menu
        }
    }
}

// Draws the pause menu (overlay and buttons)
void Pause::draw() {
    window.draw(overlay);
    resumeButton.draw(window);
    mainMenuButton.draw(window);
}