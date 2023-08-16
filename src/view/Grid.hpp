
#ifndef GRID_HPP
#define GRID_HPP

#include "../viewmodel/GameViewModel.hpp"
#include "../common/Constants.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <unordered_map>

class Grid {
public:
    explicit Grid(GameViewModel& viewModel);
    ~Grid();

    void render(SDL_Renderer* renderer);
    void handleMouseClick(int x, int y);
    void clearTextureCache();

private:
    SDL_Texture* getOrCreateTexture(SDL_Renderer* renderer, const std::string& imagePath);
    void createGridTexture(SDL_Renderer* renderer);

    GameViewModel& m_viewModel;

    SDL_Texture* m_gridTexture;
    std::unordered_map<std::string, SDL_Texture*> m_textureCache;
};

#endif // GRID_HPP

