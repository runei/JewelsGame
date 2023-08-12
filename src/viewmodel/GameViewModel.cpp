#include "GameViewModel.hpp"

GameViewModel::GameViewModel() : m_score(0) {
    // Initialize game state and gems
}

void GameViewModel::Update() {
    // Update game logic, handle interactions, etc.
    // Notify observers about changes, if applicable
    // NotifyObservers();
}

// const std::vector<Gem>& GameViewModel::GetGems() const {
//     return m_gems;
// }

int GameViewModel::GetScore() const {
    return m_score;
}
