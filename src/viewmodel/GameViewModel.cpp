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


    for (int row = 0; row < m_numRows; ++row) {
        for (int col = 0; col < m_numCols; ++col) {
            Colour randomColour = m_colourManager.getRandomColour();

            // Ensure no immediate horizontal or vertical match
            while (col >= 2 && m_grid[row][col - 1].getColour() == randomColour && m_grid[row][col - 2].getColour() == randomColour) {
                randomColour = m_colourManager.getRandomColour();
            }

            while (row >= 2 && m_grid[row - 1][col].getColour() == randomColour && m_grid[row - 2][col].getColour() == randomColour) {
                randomColour = m_colourManager.getRandomColour();
            }

            m_grid[row][col] = Jewel(randomColour);
        }
    }
}

bool GameViewModel::removeMatches() {

    // List to keep track of jewels to be removed
    std::vector<Jewel *> jewelsToRemove;

    // Check vertically
    for (int col = 0; col < m_numCols; ++col) {
        int row = 0;
        while (row < m_numRows - 2) {
            Colour color = m_grid[row][col].getColour();
            if (color != Colour::Unknown) {
                int matchCount = 1;
                for (int i = 1; row + i < m_numRows && m_grid[row + i][col].getColour() == color; ++i) {
                    ++matchCount;
                }
                if (matchCount >= 3) {
                    for (int i = 0; i < matchCount; ++i) {
                        jewelsToRemove.push_back(&m_grid[row + i][col]);
                    }
                }
                row += matchCount;
            } else {
                ++row;
            }
        }
    }

    // Check horizontally
    for (int row = 0; row < m_numRows; ++row) {
        int col = 0;
        while (col < m_numCols - 2) {
            Colour color = m_grid[row][col].getColour();
            if (color != Colour::Unknown) {
                int matchCount = 1;
                for (int i = 1; col + i < m_numCols && m_grid[row][col + i].getColour() == color; ++i) {
                    ++matchCount;
                }
                if (matchCount >= 3) {
                    for (int i = 0; i < matchCount; ++i) {
                        jewelsToRemove.push_back(&m_grid[row][col + i]);
                    }
                }
                col += matchCount;
            } else {
                ++col;
            }
        }
    }

    // Set the color of matched jewels to Unknown
    for (Jewel *jewel : jewelsToRemove) {
        jewel->setColour(Colour::Unknown);
    }

    collapseEmptySpaces();

    return !jewelsToRemove.empty();
}

bool GameViewModel::collapseEmptySpaces() {
    bool anySwap = false;

    for (int col = 0; col < m_numCols; ++col) {
        int destRow = m_numRows - 1;
        for (int row = m_numRows - 1; row >= 0; --row) {
            Colour color = m_grid[row][col].getColour();
            if (color == Colour::Unknown) {
                // Find the first non-empty jewel in the column above
                int swapRow = row - 1;
                while (swapRow >= 0 && m_grid[swapRow][col].getColour() == Colour::Unknown) {
                    --swapRow;
                }
                if (swapRow >= 0) {
                    std::swap(m_grid[row][col], m_grid[swapRow][col]);
                    anySwap = true;
                }
            }
        }
    }

    return anySwap;
}

// Add this function to fill empty spaces with random colors
void GameViewModel::fillEmptySpacesWithRandomColors() {
    for (int row = m_numRows - 1; row >= 0; --row) {
        for (int col = 0; col < m_numCols; ++col) {
            if (getJewelColour(row, col) == Colour::Unknown) {
                Colour randomColor = m_colourManager.getRandomColour();
                setJewelColour(row, col, randomColor);
            }
        }
    }
}
