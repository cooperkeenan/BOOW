// Pause.h
#pragma once

#include <SFML/Graphics.hpp>
#include "Button.h"
#include "GameState.h"

class Pause {
public:
    // Constructor: Initializes the pause menu with the window and font
    Pause(sf::RenderWindow& window, sf::Font& font);

    // Handles events specific to the pause menu
    void handleEvent(const sf::Event& event, GameState& currentState);

    // Draws the pause overlay and Resume button
    void draw();

private:
    sf::RenderWindow& window;            // Reference to the main window
    sf::RectangleShape overlay;          // Semi-transparent overlay
    Button resumeButton;                 // Resume button
};