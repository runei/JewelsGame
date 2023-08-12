#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

class Constants {
public:
    // Screen dimensions
    static constexpr int SCREEN_WIDTH = 800;
    static constexpr int SCREEN_HEIGHT = 600;

    // Grid dimensions
    static constexpr int GRID_ROWS = 8;
    static constexpr int GRID_COLS = 8;

    // Game-related constants
    static constexpr int INITIAL_SCORE = 0;

private:
    Constants(); // Prevent instantiation
};

#endif // CONSTANTS_HPP
