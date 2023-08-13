#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../viewmodel/GameViewModel.hpp"

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
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_backgroundTexture;

    GameViewModel m_gameViewModel;

    bool m_isRunning;

    void initializeImages();
    void renderBackground();
};

#endif // GAME_HPP
