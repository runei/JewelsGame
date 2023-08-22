#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../viewmodel/GameViewModel.hpp"
#include "../common/Constants.hpp"
#include "../components/Button.hpp"
#include "Grid.hpp"
#include "Scoreboard.hpp"


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
    void renderEndgameScreen();

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
    Scoreboard m_scoreboard;

    Button m_newGameButton;
    Button m_exitButton;

    bool m_isRunning;
    int m_collapseTimer;


    // Initialization functions
    void initializeImages();
    void renderBackground();

    // Calculate and wait for the remaining time to achieve desired frame rate
    void delayIfNeeded();
    bool initializeResources();
    void resetTimer();
    void updateCollapseAndFill(const Uint32 deltaTime);
};

#endif // GAME_HPP
