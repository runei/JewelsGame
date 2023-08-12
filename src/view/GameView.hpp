#ifndef GAME_VIEW_HPP
#define GAME_VIEW_HPP

#include "SDLRenderer.hpp"
#include "../viewmodel/GameViewModel.hpp"

class GameView {
public:
    GameView(SDLRenderer& renderer);
    ~GameView();

    void Render(const GameViewModel& viewModel);

private:
    SDLRenderer& m_renderer;
};

#endif // GAME_VIEW_HPP
