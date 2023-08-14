#include "Jewel.hpp"

Jewel::Jewel(Colour colour, int row, int col) : m_colour(colour), m_row(row), m_col(col), m_texture(nullptr) {

	// m_texture = LoadTextureForColour(m_colour);

}

Jewel::~Jewel() {

    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
}

Colour Jewel::getColour() const {
    return m_colour;
}

int Jewel::getRow() const {
    return m_row;
}

int Jewel::getCol() const {
    return m_col;
}

void Jewel::render(SDL_Renderer* renderer) {
    // Render the jewel using the provided renderer and texture
    if (m_texture) {
        // SDL_Rect destRect = { m_col * JEWEL_SIZE, m_row * JEWEL_SIZE, JEWEL_SIZE, JEWEL_SIZE };
        // SDL_RenderCopy(renderer, m_texture, nullptr, &destRect);
    }
}
