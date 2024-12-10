#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "GameState.h"

class Menu {
public:
    Menu(sf::RenderWindow& window, sf::Font& font);
    void handleEvent(sf::Event& event, GameState& currentState);
    void draw(GameState currentState);  // Modified to take GameState as a parameter

private:
    sf::RenderWindow& window;
    Button levelsButton;
    Button quitButton;
    Button level1Button;
    Button level2Button;
    Button level3Button;
};