#pragma once
#include <SFML/Graphics.hpp>
#include "GameSetup.h"

// Function to handle all game events
void handleGameEvents(sf::RenderWindow& window, sf::Event& event, GameComponents& components);

// Individual event handlers for game states
void handleLevelCompleteEvent(sf::Event& event, GameComponents& components);
void handleMenuEvent(sf::Event& event, GameComponents& components);
void handlePlayingStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components);
void handlePausedStateEvent(sf::Event& event, GameComponents& components);
void handleControlsStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components);
void handleEscapeKeyEvent(sf::Event& event, GameComponents& components);
