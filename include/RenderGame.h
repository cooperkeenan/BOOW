#pragma once

#include <SFML/Graphics.hpp>
#include "GameSetup.h"

// Function to handle rendering and updating the game state
void renderGameState(GameComponents& components, sf::RenderWindow& window);

// Individual state handlers
void handleMainMenuOrLevelSelection(GameComponents& components, sf::RenderWindow& window);
void handlePlayingState(GameComponents& components, sf::RenderWindow& window);
void handlePausedState(GameComponents& components, sf::RenderWindow& window);
void handleControlsState(GameComponents& components, sf::RenderWindow& window);
void handleLevelCompleteState(GameComponents& components, sf::RenderWindow& window);

// Helper function for clearing the window
void clearWindow(sf::RenderWindow& window);
