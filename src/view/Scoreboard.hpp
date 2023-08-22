#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include "../viewmodel/GameViewModel.hpp"
#include "../common/Constants.hpp"
#include "../components/Button.hpp"
#include "Grid.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <string>

class Scoreboard {
public:
    // Constructor
    Scoreboard(SDL_Renderer** renderer, GameViewModel& gameViewModel, Grid& grid);

    // Destructor
    ~Scoreboard();

    // Render the scoreboard
    void render();

    // Handle mouse click events within the scoreboard
    void handleMouseClick(const SDL_Event& event);

private:
    SDL_Renderer** m_renderer;
    GameViewModel& m_gameViewModel;
    Grid& m_grid;

    Button m_newGameButton;

    // Render the background of the scoreboard
    void renderBackground();

    // Render text at the specified position
    void write(const std::string& text, const double xpos, const double ypos);
    void writeScore(const double xpos, const double ypos);

    // Calculate position and size based on an offset
    int getXPos(const double offset) const;
    int getYPos(const double offset) const;
    int getWidth(const double offset) const;
    int getHeight(const double offset) const;
};

#endif // SCOREBOARD_HPP
