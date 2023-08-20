#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include "../viewmodel/GameViewModel.hpp"
#include "../common/Constants.hpp"
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
    TTF_Font* m_font;

    GameViewModel& m_gameViewModel;

    void renderBackground();

    void write(const std::string& text, const double xpos, const double ypos);

    void writeScore(const double xpos, const double ypos);

    int getXPos(const double offset) const;
    int getYPos(const double offset) const;

};

#endif // SCOREBOARD_HPP
