#pragma once

#include <SFML/Graphics.hpp>
#include "GameSetup.h"

// Manages rendering and updating the game based on the current game state.
void renderGameState(GameComponents& components, sf::RenderWindow& window);

// Handles rendering and logic for the Main Menu and Level Selection states.
void handleMainMenuOrLevelSelection(GameComponents& components, sf::RenderWindow& window);

// Handles rendering and logic for the Playing state.
void handlePlayingState(GameComponents& components, sf::RenderWindow& window);

// Handles rendering and logic for the Paused state.
void handlePausedState(GameComponents& components, sf::RenderWindow& window);

// Handles rendering and logic for the Controls state.
void handleControlsState(GameComponents& components, sf::RenderWindow& window);

// Handles rendering and logic for the Level Complete state.
void handleLevelCompleteState(GameComponents& components, sf::RenderWindow& window);

// Clears the game window.
void clearWindow(sf::RenderWindow& window);