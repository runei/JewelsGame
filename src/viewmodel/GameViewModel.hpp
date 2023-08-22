#ifndef GAME_VIEW_MODEL_HPP
#define GAME_VIEW_MODEL_HPP

#include "../model/Jewel.hpp"
#include "../model/Timer.hpp"
#include "../common/ColourManager.hpp"
#include <vector>
#include <string>
#include <utility>

class GameViewModel {
public:
    GameViewModel(int numRows, int numCols);

    // Getters
    int getNumRows() const;
    int getNumCols() const;
    std::string getColourImgPath(int row, int col) const;
    bool isJewelHighlighted(int row, int col) const;
    bool isColourUnknown(int row, int col) const;

    // Interaction functions
    bool toggleJewelHighlight(int row, int col);
    void swapJewels(const std::pair<int, int>& pos1, const std::pair<int, int>& pos2);
    bool removeMatches();
    bool collapseEmptySpaces();
    bool fillEmptySpacesWithRandomColors();
    bool rollbackSwap();
    void reset();
    bool isGameOver() const;

    // Data retrieval
    int getScore() const;
    int getTimeRemaining() const;

private:
    std::vector<std::vector<Jewel>> m_grid;
    ColourManager m_colourManager;
    Timer m_timer;

    static constexpr int INVALID = -1;

    std::pair<int, int> m_highlighted;
    std::pair<int, int> m_gridSize;
    std::pair<std::pair<int, int>, std::pair<int, int>> m_swapped;

    int m_score;

    Colour getJewelColour(int row, int col) const;
    void setJewelColour(int row, int col, Colour colour);

    std::pair<int, int> getInvalidPair();
    bool isPairInvalid(const std::pair<int, int>& pair);

    void fillGridRandomly();
    Colour getRandomColorWithoutMatches(int row, int col);
    bool hasHorizontalOrVerticalMatch(int row, int col, const Colour& colour) const;
    void resetGrid();
    void resetTime();
    void resetScore();
    void addTime(int scoreAdded);
    int findNextNonEmptyRow(int currentRow, int col) const;
};

#endif // GAME_VIEW_MODEL_HPP
