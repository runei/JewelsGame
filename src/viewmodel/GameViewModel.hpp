#ifndef GAME_VIEW_MODEL_HPP
#define GAME_VIEW_MODEL_HPP

#include "../model/Jewel.hpp"
// #include "Observer.h"
// #include "Subject.h"
#include <vector>

//This class will manage the game state and provide an interface for the view to access and update that state.
class GameViewModel //: public Subject
{
public:
    GameViewModel();

    void update();
    const std::vector<Jewel>& getJewels() const;
    int getScore() const;

private:
    std::vector<Jewel> m_jewels;
    int m_score;

};

#endif // GAME_VIEW_MODEL_HPP
