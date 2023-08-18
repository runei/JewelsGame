#include "GameViewModel.hpp"
#include <random>
#include "../external/loguru.hpp"

GameViewModel::GameViewModel(int numRows, int numCols) : m_numRows(numRows), m_numCols(numCols), m_grid(numRows, std::vector<Jewel>(numCols)), m_highlightedRow(NOT_HIGHLIGHTED), m_highlightedCol(NOT_HIGHLIGHTED) {
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

bool GameViewModel::isJewelHighlighted(int row, int col) const {
    return m_grid[row][col].isHighlighted();
}

bool GameViewModel::isColourUnknown(int row, int col) const {
    return getJewelColour(row, col) == Colour::Unknown;
}

bool GameViewModel::toggleJewelHighlight(int row, int col) {
    Jewel& jewel = m_grid[row][col];


    if (jewel.isHighlighted()) {
        LOG_F(INFO, "Unhighlighting jewel at row %d, col %d", row, col);
        jewel.setHighlighted(false);
        m_highlightedRow = NOT_HIGHLIGHTED;
        m_highlightedCol = NOT_HIGHLIGHTED;
        return true;
    } else {
        if (m_highlightedRow != NOT_HIGHLIGHTED && m_highlightedCol != NOT_HIGHLIGHTED) {
            if ((row == m_highlightedRow && abs(col - m_highlightedCol) == 1) ||
                (col == m_highlightedCol && abs(row - m_highlightedRow) == 1)) {
                LOG_F(INFO, "Swapping jewels at row %d, col %d with row %d, col %d", m_highlightedRow, m_highlightedCol, row, col);
                swapJewels(m_highlightedRow, m_highlightedCol, row, col);
            }
        } else {
            LOG_F(INFO, "Highlighting jewel at row %d, col %d", row, col);
            jewel.setHighlighted(true);
            m_highlightedRow = row;
            m_highlightedCol = col;
        }
        return true;
    }

    return false;
}


void GameViewModel::swapJewels(int row1, int col1, int row2, int col2) {
    std::swap(m_grid[row1][col1], m_grid[row2][col2]);

    m_grid[row1][col1].setHighlighted(false);
    m_grid[row2][col2].setHighlighted(false);

    m_highlightedRow = NOT_HIGHLIGHTED;
    m_highlightedCol = NOT_HIGHLIGHTED;
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

            // Ensure no immediate horizontal or vertical match
            while (col >= 2 && m_grid[row][col - 1].getColour() == randomColour && m_grid[row][col - 2].getColour() == randomColour) {
                randomColour = static_cast<Colour>(colourDistribution(gen));
            }

            while (row >= 2 && m_grid[row - 1][col].getColour() == randomColour && m_grid[row - 2][col].getColour() == randomColour) {
                randomColour = static_cast<Colour>(colourDistribution(gen));
            }

            m_grid[row][col] = Jewel(randomColour);
        }
    }
}

bool GameViewModel::removeMatches() {
    bool anyMatches = false;

    // Check horizontally
    for (int row = 0; row < m_numRows; ++row) {
        for (int col = 0; col < m_numCols - 2; ++col) {
            Colour color = m_grid[row][col].getColour();
            if (color != Colour::Unknown &&
                m_grid[row][col + 1].getColour() == color &&
                m_grid[row][col + 2].getColour() == color) {
                m_grid[row][col].setColour(Colour::Unknown);
                m_grid[row][col + 1].setColour(Colour::Unknown);
                m_grid[row][col + 2].setColour(Colour::Unknown);
                anyMatches = true;
            }
        }
    }

    // Check vertically
    for (int col = 0; col < m_numCols; ++col) {
        for (int row = 0; row < m_numRows - 2; ++row) {
            Colour color = m_grid[row][col].getColour();
            if (color != Colour::Unknown &&
                m_grid[row + 1][col].getColour() == color &&
                m_grid[row + 2][col].getColour() == color) {
                m_grid[row][col].setColour(Colour::Unknown);
                m_grid[row + 1][col].setColour(Colour::Unknown);
                m_grid[row + 2][col].setColour(Colour::Unknown);
                anyMatches = true;
            }
        }
    }

    return anyMatches;
}
