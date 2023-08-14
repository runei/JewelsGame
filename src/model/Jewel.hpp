#ifndef JEWEL_HPP
#define JEWEL_HPP

#include "SDL2/SDL.h"
#include "Colour.hpp"

// Represents a coloured jewel in the game grid
class Jewel {
public:
    Jewel(Colour colour, int row, int col);
    ~Jewel();

    // Getters
    Colour getColour() const;
    int getRow() const;
    int getCol() const;

    void render(SDL_Renderer* renderer);

private:
    Colour m_colour;
    int m_row;
    int m_col;
    SDL_Texture* m_texture;

};

#endif // JEWEL_HPP
