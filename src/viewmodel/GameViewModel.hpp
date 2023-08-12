#ifndef GAME_VIEW_MODEL_HPP
#define GAME_VIEW_MODEL_HPP

// #include "Jewel.h"  
// #include "Observer.h"
// #include "Subject.h"
#include <vector>

//This class will manage the game state and provide an interface for the view to access and update that state.
class GameViewModel //: public Subject 
{
public:
    GameViewModel();

    void Update();
    // const std::vector<Jewel>& GetJewels() const;
    int GetScore() const;

private:
    // std::vector<Jewel> m_jewels;
    int m_score;

};

#endif // GAME_VIEW_MODEL_HPP
