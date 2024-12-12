#pragma once
#include <SFML/Graphics.hpp>
#include "GameSetup.h"
#include "Menu.h"

// Function to handle all game events
void handleGameEvents(sf::RenderWindow& window, sf::Event& event, GameComponents& components);

// Individual event handlers for game states
void handleLevelCompleteEvent(sf::Event& event, GameComponents& components);
void handleMenuEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components); // Correct: Takes window
void handlePlayingStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components);
void handlePausedStateEvent(sf::Event& event, GameComponents& components);
void handleControlsStateEvent(sf::RenderWindow& window, sf::Event& event, GameComponents& components);
void handleEscapeKeyEvent(sf::Event& event, GameComponents& components);
