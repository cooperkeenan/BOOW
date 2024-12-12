#pragma once
#include <SFML/Graphics.hpp>
#include "GameSetup.h"
#include "Menu.h"

// Handles all game events based on the current game state.
void handleGameEvents(sf::RenderWindow& window, sf::Event& event, GameComponents& components);

// Handles events specific to the Level Complete state.
void handleLevelCompleteEvent(sf::Event& event, GameComponents& components);

// Handles events specific to the Main Menu and Level Selection states.
void handleMenuEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components);

// Handles events specific to the Playing state.
void handlePlayingStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components);

// Handles events specific to the Paused state.
void handlePausedStateEvent(sf::Event& event, GameComponents& components);

// Handles events specific to the Controls state.
void handleControlsStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components);

// Handles Escape key presses for pausing and resuming the game.
void handleEscapeKeyEvent(sf::Event& event, GameComponents& components);