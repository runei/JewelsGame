#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "../common/Constants.hpp"
#include "../viewmodel/GameViewModel.hpp"

class Grid {
public:
    Grid(GameViewModel& viewModel);

    void render(SDL_Renderer* renderer, int xOffset, int yOffset);

    void handleMouseClick(int mouseX, int mouseY);

private:

	int m_highlightedRow;
    int m_highlightedCol;

    GameViewModel& m_viewModel;

    SDL_Texture* renderJewel(SDL_Renderer* renderer, const std::string& imagePath);
};

#endif // GRID_HPP
