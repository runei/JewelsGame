#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

class Constants {
public:
	static constexpr const char* GAME_NAME = "Jewel Game";

    // Configuration
    static constexpr const char* COLOUR_CONFIG_PATH = "config/ColourConfig.json";

    // Frames
    static constexpr int FRAME_RATE = 120;
    static constexpr int TIME_PER_FRAME = 1000 / FRAME_RATE;


    // Grid dimensions
    static constexpr int GRID_ROWS = 8;
    static constexpr int GRID_COLS = 8;
    static constexpr int JEWEL_SIZE = 64;

    // Screen dimensions
    static constexpr int SCREEN_WIDTH = 800;//JEWEL_SIZE * GRID_ROWS; //800
    static constexpr int SCREEN_HEIGHT = 800;//JEWEL_SIZE * GRID_COLS; //600

    // Offsets for grid in the screen
    static constexpr int OFFSET_ROW = 0;
    static constexpr int OFFSET_COL = 0;

    // Game-related constants
    static constexpr int INITIAL_SCORE = 0;

private:
    Constants(); // Prevent instantiation
};

#endif // CONSTANTS_HPP
