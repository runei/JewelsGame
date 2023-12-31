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
    SDL_Surface* m_icon;

    GameViewModel m_gameViewModel;
    Grid m_grid;
    Scoreboard m_scoreboard;

    Button m_newGameButton;
    Button m_exitButton;

    bool m_isRunning;
    int m_collapseTimer;


    // Initialization functions
    void initializeImages();

    void resetTimer();
    void updateCollapseAndFill(const Uint32 deltaTime);

    void handleQuitEvent();
    void handleMouseButtonDownEvent(const SDL_Event& event);
    void handleMouseMotionEvent(const SDL_Event& event);
    void handleMouseButtonUpEvent(const SDL_Event& event);
    void handleGameOverEvent(const SDL_Event& event);

};

#endif // GAME_HPP
