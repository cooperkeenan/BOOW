// GameState.h
#pragma once

enum class GameState {
    MainMenu,
    LevelSelection,
    Playing,
    Paused,
    LevelComplete,
    Controls
};

enum class LevelResult {
    Complete,
    Failed
};
