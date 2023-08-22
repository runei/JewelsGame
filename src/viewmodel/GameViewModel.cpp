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
    const double timeToAdd = (double) scoreAdded / 3.0;
    m_timer.addTime(timeToAdd);
}

// Interaction function to toggle jewel highlight
bool GameViewModel::toggleJewelHighlight(int row, int col) {
    Jewel& jewel = m_grid[row][col];

    if (jewel.isHighlighted()) {
        LOG_F(INFO, "Unhighlighting jewel at row %d, col %d", row, col);
        jewel.setHighlighted(false);
        m_highlighted = getInvalidPair();
    } else {
        if (!isPairInvalid(m_highlighted)) {
            int highlightedRow = m_highlighted.first;
            int highlightedCol = m_highlighted.second;

            if ((row == highlightedRow && abs(col - highlightedCol) == 1) ||
                (col == highlightedCol && abs(row - highlightedRow) == 1)) {
                LOG_F(INFO, "Swapping jewels at row %d, col %d with row %d, col %d", highlightedRow, highlightedCol, row, col);
                swapJewels(m_highlighted, std::make_pair(row, col));
            }
        } else {
            LOG_F(INFO, "Highlighting jewel at row %d, col %d", row, col);
            jewel.setHighlighted(true);
            m_highlighted = std::make_pair(row, col);
        }
    }

    return true;
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
    Colour randomColour;

    do {
        randomColour = m_colourManager.getRandomColour();
    } while (hasHorizontalOrVerticalMatch(row, col, randomColour));

    return randomColour;
}


bool GameViewModel::hasHorizontalOrVerticalMatch(int row, int col, const Colour& colour) const {
    if (col >= 2 && (m_grid[row][col - 1].getColour() == colour && m_grid[row][col - 2].getColour() == colour)) {
        return true;
    }

    if (row >= 2 && (m_grid[row - 1][col].getColour() == colour && m_grid[row - 2][col].getColour() == colour)) {
        return true;
    }

    return false;
}


bool GameViewModel::isGameOver() const {
    return m_timer.getTimeRemaining() <= 0.0;
}

void GameViewModel::resetGrid() {
    for (auto& row : m_grid) {
        for (auto& jewel : row) {
            jewel.setColour(Colour::Unknown);
        }
    }
}


void GameViewModel::resetTime() {
    m_timer.start(Constants::INITIAL_TIME);
}

void GameViewModel::resetScore() {
    m_score = 0;
}

void GameViewModel::reset() {
    resetGrid();
    fillGridRandomly();
    resetTime();
    resetScore();
}

bool GameViewModel::removeMatches() {
    std::vector<Jewel *> jewelsToRemove;
    int scoreToAdd = 0;

    for (int row = 0; row < m_gridSize.first; ++row) {
        for (int col = 0; col < m_gridSize.second; ++col) {
            Colour color = m_grid[row][col].getColour();
            if (color != Colour::Unknown) {
                int verticalMatchCount = 1;
                int horizontalMatchCount = 1;

                // Check for vertical match
                for (int i = 1; row + i < m_gridSize.first && m_grid[row + i][col].getColour() == color; ++i) {
                    ++verticalMatchCount;
                }

                // Check for horizontal match
                for (int i = 1; col + i < m_gridSize.second && m_grid[row][col + i].getColour() == color; ++i) {
                    ++horizontalMatchCount;
                }

                if (verticalMatchCount >= 3) {
                    for (int i = 0; i < verticalMatchCount; ++i) {
                        jewelsToRemove.push_back(&m_grid[row + i][col]);
                    }
                }

                if (horizontalMatchCount >= 3) {
                    for (int i = 0; i < horizontalMatchCount; ++i) {
                        jewelsToRemove.push_back(&m_grid[row][col + i]);
                    }
                }
            }
        }
    }

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
                int targetRow = findNextNonEmptyRow(row, col);

                if (targetRow != -1) {
                    std::swap(m_grid[targetRow][col], m_grid[row][col]);
                    anySwap = true;
                }
            }
        }
    }

    return anySwap;
}

int GameViewModel::findNextNonEmptyRow(int currentRow, int col) const {
    int row = currentRow - 1;

    while (row >= 0 && m_grid[row][col].getColour() == Colour::Unknown) {
        --row;
    }

    return row;
}

bool GameViewModel::fillEmptySpacesWithRandomColors() {
    bool wasFilled = false;
    const int targetRow = 0;

    for (int targetCol = 0; targetCol < m_gridSize.second; ++targetCol) {
        if (getJewelColour(targetRow, targetCol) == Colour::Unknown) {
            Colour randomColor = m_colourManager.getRandomColour();
            setJewelColour(targetRow, targetCol, randomColor);
            wasFilled = true;
        }
    }

    return wasFilled;
}
