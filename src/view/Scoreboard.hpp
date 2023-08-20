#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include "../viewmodel/GameViewModel.hpp"
#include "../common/Constants.hpp"
#include "../components/Button.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <string>

class Scoreboard {
public:
    Scoreboard(SDL_Renderer** renderer, GameViewModel& gameViewModel);
	~Scoreboard();


    void update();
    void render();

private:
    SDL_Renderer** m_renderer;
    GameViewModel& m_gameViewModel;

    Button m_newGameButton;

    void renderBackground();

    void write(const std::string& text, const double xpos, const double ypos);

    void writeScore(const double xpos, const double ypos);

    // The offset is in relation to the JEWEL_SIZE
    int getXPos(const double offset) const;
    int getYPos(const double offset) const;
    int getWidth(const double offset) const;
    int getHeight(const double offset) const;

};

#endif // SCOREBOARD_HPP
