#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace Constants {

    constexpr const char* GAME_NAME = "Jewel Game";

    // Configuration
    constexpr const char* COLOUR_CONFIG_PATH = "config/ColourConfig.json";

    // Frames
    constexpr int FRAME_RATE = 60;
    constexpr int TIME_PER_FRAME = 1000 / FRAME_RATE;

    // Grid dimensions
    constexpr int GRID_ROWS = 8;
    constexpr int GRID_COLS = 8;
    constexpr int JEWEL_SIZE = 64;

    // Screen dimensions
    constexpr int SCREEN_WIDTH = JEWEL_SIZE * (GRID_ROWS + 4);
    constexpr int SCREEN_HEIGHT = JEWEL_SIZE * GRID_COLS;

    // Game-related constants
    constexpr int INITIAL_SCORE = 0;
    constexpr int INITIAL_TIME = 60;
    constexpr int MATCH_THRESHOLD = 3;

    // Layout related
    constexpr int BUTTON_FONT_SIZE = 22;

} // namespace GameConstants

#endif // CONSTANTS_HPP
