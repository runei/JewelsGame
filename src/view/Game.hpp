#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../viewmodel/GameViewModel.hpp"
#include "../common/Constants.hpp"
#include "Grid.hpp"


class Game {
public:
    Game();
    ~Game();

    // Initialize the game
    bool initialize(const char* title, const int width, const int height);

    // Handle user input and update game logic
    void handleEvents();
    void update();

    // Render the game
    void render();

    // Clean up resources
    void cleanup();

    bool isRunning();

private:

    Uint32 m_prevFrameTime;

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_backgroundTexture;

    GameViewModel m_gameViewModel;
    Grid m_grid;

    bool m_isRunning;

    // Initialization functions
    bool initializeSDL();
    bool createWindowAndRenderer(const char* title, const int width, const int height);
    void initializeImages();
    void renderBackground();

    // Calculate and wait for the remaining time to achieve desired frame rate
    void delayIfNeeded();
};

#endif // GAME_HPP
