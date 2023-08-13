#include "GameViewModel.hpp"

GameViewModel::GameViewModel() : m_score(0) {
    // Initialize game state and gems
}

void GameViewModel::update() {
    // Update game logic, handle interactions, etc.
    // Notify observers about changes, if applicable
    // NotifyObservers();
}

const std::vector<Jewel>& GameViewModel::getJewels() const {
    return m_jewels;
}

int GameViewModel::getScore() const {
    return m_score;
}
