#pragma once

#include <SFML/Graphics.hpp>
#include "Button.h"
#include "GameState.h"

// Represents the pause menu overlay.
class Pause {
public:
    Pause(sf::RenderWindow& window, sf::Font& font); // Constructor: Initializes the pause menu.
    void handleEvent(const sf::Event& event, GameState& currentState); // Handles events within the pause menu.
    void draw();                                                      // Draws the pause menu overlay and buttons.

private:
    sf::RenderWindow& window;    // Reference to the game window.
    sf::RectangleShape overlay; // Semi-transparent overlay displayed during pause.
    Button resumeButton;      // Button to resume the game.
    Button mainMenuButton;    // Button to return to the main menu.
};