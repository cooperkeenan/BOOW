#pragma once

// Enumeration representing the different states of the game.
enum class GameState {
    MainMenu,       // The main menu state.
    LevelSelection, // The level selection screen state.
    Playing,        // The main gameplay state.
    Paused,         // The game paused state.
    LevelComplete,  // The level complete (win or lose) state.
    Controls        // The game controls screen state.
};

// Enumeration representing the result of a level.
enum class LevelResult {
    Complete, // The level was successfully completed.
    Failed    // The level was failed.
};