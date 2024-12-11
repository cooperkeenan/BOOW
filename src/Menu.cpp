#include <iostream>
#include "Menu.h"
#include "Constants.h"


Menu::Menu(sf::RenderWindow& window, sf::Font& font) : window(window),
    levelsButton("Levels", {200, 50}, 20, sf::Color::Blue, sf::Color::White),
    quitButton("Quit", {200, 50}, 20, sf::Color::Red, sf::Color::White),
    level1Button("Level 1", {200, 50}, 20, sf::Color::Green, sf::Color::White),
    level2Button("Level 2", {200, 50}, 20, sf::Color::Green, sf::Color::White),
    level3Button("Level 3", {200, 50}, 20, sf::Color::Green, sf::Color::White),
    controlsButton("Controls", {200, 50}, 20, sf::Color::Yellow, sf::Color::White)  // Add this
{
    // Set font and position for each button
    levelsButton.setFont(font);
    levelsButton.setPosition({300, 200});
    quitButton.setFont(font);
    quitButton.setPosition({300, 300});
    level1Button.setFont(font);
    level1Button.setPosition({300, 150});
    level2Button.setFont(font);
    level2Button.setPosition({300, 250});
    level3Button.setFont(font);
    level3Button.setPosition({300, 350});
    controlsButton.setFont(font);  // Set font for controls button
    controlsButton.setPosition({300, 100});  // Set position for controls button
}

void Menu::handleEvent(sf::Event& event, GameState& currentState) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (currentState == GameState::MainMenu) {
            if (quitButton.isMouseOver(window)) {
                window.close();
            } else if (levelsButton.isMouseOver(window)) {
                currentState = GameState::LevelSelection;
            } else if (controlsButton.isMouseOver(window)) { // Handle Controls Button
                currentState = GameState::Controls;
            }
        } else if (currentState == GameState::LevelSelection) {
            if (level1Button.isMouseOver(window)) {
                currentState = GameState::Playing;
                std::cout << "Starting Level 1" << std::endl;
            } else if (level2Button.isMouseOver(window)) {
                std::cout << "Starting Level 2" << std::endl;
            } else if (level3Button.isMouseOver(window)) {
                std::cout << "Starting Level 3" << std::endl;
            }
        }
    }
}
void Menu::draw(GameState currentState) {
    // Draw only relevant buttons based on the current state
    if (currentState == GameState::MainMenu) {
        levelsButton.draw(window);
        controlsButton.draw(window);  // Draw the controls button
        quitButton.draw(window);
    } else if (currentState == GameState::LevelSelection) {
        level1Button.draw(window);
        level2Button.draw(window);
        level3Button.draw(window);
    }
}


void Menu::drawLevelCompleteScreen(sf::RenderWindow& window) {
    sf::Text text("Level Complete!", font, 50);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.width / 2, textRect.height / 2);
    text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));

    window.draw(text);  // Draw the "Level Complete" text
}