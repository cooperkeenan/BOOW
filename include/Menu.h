#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "GameState.h"

class Menu {
public:
    Menu(sf::RenderWindow& window, sf::Font& font);
    void handleEvent(sf::Event& event, GameState& currentState);
    void handleLevelCompleteEvent(sf::Event& event, GameState& currentState);
    void draw(GameState currentState);
    void drawLevelCompleteScreen();

private:
    sf::RenderWindow& window;
    sf::Font& font;  // Store a reference to the font
    Button levelsButton;
    Button controlsButton;
    Button quitButton;
    Button level1Button;
    Button level2Button;
    Button level3Button;
};
