#include "Jewel.hpp"

// Default constructor
Jewel::Jewel() : m_colour(Colour::Unknown), m_highlighted(false) {}

// Constructor with colour assignment
Jewel::Jewel(Colour colour) : m_colour(colour), m_highlighted(false) {}

// Getter for colour
Colour Jewel::getColour() const {
    return m_colour;
}

// Setter for colour
void Jewel::setColour(Colour colour) {
    m_colour = colour;
}

// Getter for highlighting
bool Jewel::isHighlighted() const {
    return m_highlighted;
}

// Setter for highlighting
void Jewel::setHighlighted(bool highlighted) {
    m_highlighted = highlighted;
}
