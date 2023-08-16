#include "Jewel.hpp"

Jewel::Jewel() : m_colour(Colour::Unknown) {}

Jewel::Jewel(Colour colour) : m_colour(colour) {}

Colour Jewel::getColour() const {
    return m_colour;
}

void Jewel::setColour(Colour colour) {
    m_colour = colour;
}

bool Jewel::isHighlighted() const {
	return m_highlighted;
}

void Jewel::setHighlighted(bool highlighted) {
	m_highlighted = highlighted;
}
