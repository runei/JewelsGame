#include "GameViewModel.hpp"
#include "../external/loguru.hpp"
#include "../common/Constants.hpp"
#include <random>

GameViewModel::GameViewModel(int numRows, int numCols) : m_gridSize(numRows, numCols), m_grid(numRows, std::vector<Jewel>(numCols)), m_highlighted(getInvalidPair()), m_swapped(getInvalidPair(), getInvalidPair()) {

	fillGridRandomly();

    resetTime();
    resetScore();
}

int GameViewModel::getNumRows() const {
    return m_gridSize.first;
}

int GameViewModel::getNumCols() const {
    return m_gridSize.second;
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

int GameViewModel::getScore() const {
    return m_score;
}

std::pair<int, int> GameViewModel::getInvalidPair() {
    return std::make_pair(INVALID, INVALID);
}

bool GameViewModel::isPairInvalid(const std::pair<int, int>& pair) {
    return pair.first == INVALID || pair.second == INVALID;
}

int GameViewModel::getTimeRemaining() const {
    return (int) m_timer.getTimeRemaining();
}

void GameViewModel::addTime(int scoreAdded) {
    const double timeToAdd = (double) scoreAdded / 2.0;
    m_timer.addTime(timeToAdd);
}

bool GameViewModel::toggleJewelHighlight(int row, int col) {
    Jewel& jewel = m_grid[row][col];


    if (jewel.isHighlighted()) {
        LOG_F(INFO, "Unhighlighting jewel at row %d, col %d", row, col);
        jewel.setHighlighted(false);
        m_highlighted = getInvalidPair();
        return true;
    } else {
        if (!isPairInvalid(m_highlighted)) {
            if ((row == m_highlighted.first && abs(col - m_highlighted.second) == 1) ||
                (col == m_highlighted.second && abs(row - m_highlighted.first) == 1)) {
                LOG_F(INFO, "Swapping jewels at row %d, col %d with row %d, col %d", m_highlighted.first, m_highlighted.second, row, col);
                swapJewels(m_highlighted, std::make_pair(row, col));
            }
        } else {
            LOG_F(INFO, "Highlighting jewel at row %d, col %d", row, col);
            jewel.setHighlighted(true);
            m_highlighted = std::make_pair(row, col);
        }
        return true;
    }

    return false;
}

void GameViewModel::swapJewels(const std::pair<int, int>& pos1, const std::pair<int, int>& pos2) {

    m_swapped = std::make_pair(pos1, pos2);

    std::swap(m_grid[pos1.first][pos1.second], m_grid[pos2.first][pos2.second]);

    m_grid[pos1.first][pos1.second].setHighlighted(false);
    m_grid[pos2.first][pos2.second].setHighlighted(false);

    m_highlighted = getInvalidPair();
}

void GameViewModel::fillGridRandomly() {
    for (int row = 0; row < m_gridSize.first; ++row) {
        for (int col = 0; col < m_gridSize.second; ++col) {
            Colour randomColour = getRandomColorWithoutMatches(row, col);
            m_grid[row][col] = Jewel(randomColour);
        }
    }
}

Colour GameViewModel::getRandomColorWithoutMatches(int row, int col) {
    Colour randomColour = m_colourManager.getRandomColour();

    while (hasHorizontalOrVerticalMatch(row, col, randomColour)) {
        randomColour = m_colourManager.getRandomColour();
    }

    return randomColour;
}

bool GameViewModel::hasHorizontalOrVerticalMatch(int row, int col, const Colour& colour) const {
    if (col >= 2 && m_grid[row][col - 1].getColour() == colour && m_grid[row][col - 2].getColour() == colour) {
        return true;
    }

    if (row >= 2 && m_grid[row - 1][col].getColour() == colour && m_grid[row - 2][col].getColour() == colour) {
        return true;
    }

    return false;
}

bool GameViewModel::isGameOver() const {
    return m_timer.getTimeRemaining() <= 0.0;
}

void GameViewModel::resetGrid() {
    for (int row = 0; row < m_gridSize.first; ++row) {
        for (int col = 0; col < m_gridSize.second; ++col) {
            m_grid[row][col].setColour(Colour::Unknown);
        }
    }
}

void GameViewModel::resetTime() {
    m_timer.start(5);
}

void GameViewModel::resetScore() {
    m_score = 0;
}

void GameViewModel::reset() {
    resetGrid();
    // fillGridRandomly();
    resetTime();
    resetScore();
}

bool GameViewModel::removeMatches() {

    // List to keep track of jewels to be removed
    std::vector<Jewel *> jewelsToRemove;

    // Check vertically
    for (int col = 0; col < m_gridSize.second; ++col) {
        int row = 0;
        while (row < m_gridSize.first - 2) {
            Colour color = m_grid[row][col].getColour();
            if (color != Colour::Unknown) {
                int matchCount = 1;
                for (int i = 1; row + i < m_gridSize.first && m_grid[row + i][col].getColour() == color; ++i) {
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
    for (int row = 0; row < m_gridSize.first; ++row) {
        int col = 0;
        while (col < m_gridSize.second - 2) {
            Colour color = m_grid[row][col].getColour();
            if (color != Colour::Unknown) {
                int matchCount = 1;
                for (int i = 1; col + i < m_gridSize.second && m_grid[row][col + i].getColour() == color; ++i) {
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

    int scoreToAdd = 0;
    // Set the color of matched jewels to Unknown
    for (Jewel *jewel : jewelsToRemove) {
        jewel->setColour(Colour::Unknown);
        scoreToAdd++;
    }
    m_score += scoreToAdd;
    addTime(scoreToAdd);

    if (!jewelsToRemove.empty()) {
        m_swapped = std::make_pair(getInvalidPair(), getInvalidPair());
        m_highlighted = getInvalidPair();
        return true;
    }
    return false;
}

bool GameViewModel::rollbackSwap() {
    if (!isPairInvalid(m_swapped.first)) {
        swapJewels(m_swapped.first, m_swapped.second);
        m_swapped = std::make_pair(getInvalidPair(), getInvalidPair());
        return true;
    }
    return false;
}

bool GameViewModel::collapseEmptySpaces() {
    bool anySwap = false;

    for (int col = 0; col < m_gridSize.second; ++col) {
       for (int row = m_gridSize.first - 1; row >= 0; --row) {
            Colour color = m_grid[row][col].getColour();
            if (color == Colour::Unknown) {
                int swapRow = row - 1;
                while (swapRow >= 0 && m_grid[swapRow][col].getColour() == Colour::Unknown) {
                    --swapRow;
                }
                if (swapRow >= 0) {
                    std::swap(m_grid[swapRow+1][col], m_grid[swapRow][col]);
                    anySwap = true;
                }
            }
        }
    }

    return anySwap;
}

bool GameViewModel::fillEmptySpacesWithRandomColors() {
    bool wasFilled = false;
    const int row = 0;

    for (int col = 0; col < m_gridSize.second; ++col) {
        if (getJewelColour(row, col) == Colour::Unknown) {
            Colour randomColor = m_colourManager.getRandomColour();
            setJewelColour(row, col, randomColor);
            wasFilled = true;
        }
    }

    return wasFilled;
}
