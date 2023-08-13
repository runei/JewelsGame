#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "../viewmodel/GameViewModel.hpp"

class Game {
public:
    Game();
    ~Game();

    // Initialize the game
    bool initialize(const char* title, const int xpos, const int ypos, const int width, const int height);

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

    GameViewModel m_gameViewModel;

    bool m_isRunning;
};

#endif // GAME_HPP
