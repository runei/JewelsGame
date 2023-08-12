#include "GameView.hpp"

GameView::GameView(SDLRenderer& renderer) : m_renderer(renderer) {}

GameView::~GameView() {
    // No cleanup needed in this example
}

void GameView::Render(const GameViewModel& viewModel) {
    m_renderer.Clear();
    
    // Render game objects and UI based on viewModel
    // Example:
    // for (const auto& gem : viewModel.GetGems()) {
    //     m_renderer.RenderTexture(gem.GetTexture(), gem.GetX(), gem.GetY(), GemWidth, GemHeight);
    // }
    
    // Render UI elements like scores, timers, etc.
    
    m_renderer.Present();
}
