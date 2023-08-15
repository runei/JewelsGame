#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "../model/Jewel.hpp"
#include "../common/Constants.hpp"
#include "../viewmodel/GameViewModel.hpp"

class Grid {
public:
    Grid(GameViewModel& viewModel);

    void render(SDL_Renderer* renderer, int xOffset, int yOffset);

private:

    GameViewModel& m_viewModel;

    SDL_Texture* renderJewel(SDL_Renderer* renderer, const std::string& imagePath);
};

#endif // GRID_HPP
