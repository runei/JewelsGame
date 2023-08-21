#ifndef GAME_VIEW_MODEL_HPP
#define GAME_VIEW_MODEL_HPP

#include "../model/Jewel.hpp"
#include "../model/Timer.hpp"
#include "../common/ColourManager.hpp"
#include <vector>
#include <string>
#include <utility>

//This class will manage the game state and provide an interface for the view to access and update that state.
class GameViewModel {
public:
    GameViewModel(int numRows, int numCols);

    // getters
    int getNumRows() const;
    int getNumCols() const;
    std::string getColourImgPath(int row, int col) const;
    bool isJewelHighlighted(int row, int col) const;
    bool isColourUnknown(int row, int col) const;

    bool toggleJewelHighlight(int row, int col);

    // game functions
    void swapJewels(const std::pair<int, int>& pos1, const std::pair<int, int>& pos2);
    bool removeMatches();
    bool collapseEmptySpaces();
    bool fillEmptySpacesWithRandomColors();
    void reset();
    int getScore() const;
    int getTimeRemaining() const;

private:
    std::vector<std::vector<Jewel>> m_grid;

    ColourManager m_colourManager;

    static constexpr int INVALID = -1;

    std::pair<int, int> m_highlighted;
    std::pair<int, int> m_gridSize;
    std::pair<int, int> m_swapped;

    int m_score;

    Timer m_timer;

    Colour getJewelColour(int row, int col) const;
    void setJewelColour(int row, int col, Colour colour);

    std::pair<int, int> getInvalidPair();
    bool isPairInvalid(const std::pair<int, int>& pair);

    void fillGridRandomly();
    void resetGrid();
    void resetTime();
    void resetScore();

};

#endif // GAMEVIEWMODEL_HPP
