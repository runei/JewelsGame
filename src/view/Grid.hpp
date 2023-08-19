
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
    void handleMouseClick(int x, int y, SDL_Renderer* renderer);
    void handleMouseMotion(int x, int y);
    void handleMouseRelease(int x, int y, SDL_Renderer* renderer);
    void clearTextureCache();
    void updateGrid();

private:
    SDL_Texture* getOrCreateTexture(SDL_Renderer* renderer, const std::string& imagePath);
    void createGridTexture(SDL_Renderer* renderer);

    GameViewModel& m_viewModel;

    SDL_Texture* m_gridTexture;
    std::unordered_map<std::string, SDL_Texture*> m_textureCache;

    bool m_dragging;
    int m_dragStartRow;
    int m_dragStartCol;
    int m_dragDestRow;
    int m_dragDestCol;

    SDL_Texture* m_dragJewelTexture;
    SDL_Texture* m_explosionTexture;

};

#endif // GRID_HPP

