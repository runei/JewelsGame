
#ifndef GRID_HPP
#define GRID_HPP

#include "../viewmodel/GameViewModel.hpp"
#include "../common/Constants.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <unordered_map>
#include <utility>

class Grid {
public:
    Grid(SDL_Renderer** renderer, GameViewModel& viewModel);
    ~Grid();

    void render();
    void handleMouseClick(int x, int y);
    void handleMouseMotion(int x, int y);
    void handleMouseRelease(int x, int y);
    void clearTextureCache();
    void updateGrid();

private:
    SDL_Texture* getOrCreateTexture(const std::string& imagePath);
    void createGridTexture();

    GameViewModel& m_viewModel;

    SDL_Renderer** m_renderer;
    SDL_Texture* m_gridTexture;
    std::unordered_map<std::string, SDL_Texture*> m_textureCache;

    bool m_dragging;
    std::pair<int, int> m_dragStart;
    std::pair<int, int> m_dragDest;

    SDL_Texture* m_dragJewelTexture;

};

#endif // GRID_HPP

