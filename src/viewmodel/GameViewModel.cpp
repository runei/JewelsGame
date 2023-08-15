#include "GameViewModel.hpp"
#include <random>

GameViewModel::GameViewModel(int numRows, int numCols) : m_numRows(numRows), m_numCols(numCols), m_grid(numRows, std::vector<Jewel>(numCols)) {
	fillGridRandomly();
}

int GameViewModel::getNumRows() const {
    return m_numRows;
}

int GameViewModel::getNumCols() const {
    return m_numCols;
}

std::string GameViewModel::getColourImgPath(int row, int col) const {
	Colour colour = getJewelColour(row, col);
	return m_colourManager.getImgPathForColour(colour);
}

Colour GameViewModel::getJewelColour(int row, int col) const {
    return m_grid[row][col].getColour();
}

void GameViewModel::setJewelColour(int row, int col, Colour colour) {
    m_grid[row][col].setColour(colour);
}

void GameViewModel::swapJewels(int row1, int col1, int row2, int col2) {
    std::swap(m_grid[row1][col1], m_grid[row2][col2]);
}

bool GameViewModel::checkForMatches() {
    // Implement match checking logic here
    return false;
}

void GameViewModel::updateGrid() {
    // Implement grid update logic here
}

void GameViewModel::fillGridRandomly() {
    const int count = static_cast<int>(Colour::Unknown);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> colourDistribution(0, count - 1);

    for (int row = 0; row < m_numRows; ++row) {
        for (int col = 0; col < m_numCols; ++col) {
            Colour randomColour = static_cast<Colour>(colourDistribution(gen));
            m_grid[row][col] = Jewel(randomColour);
        }
    }
}
