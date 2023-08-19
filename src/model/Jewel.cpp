#include "Jewel.hpp"

Jewel::Jewel() : m_colour(Colour::Unknown), m_highlighted(false) {}

Jewel::Jewel(Colour colour) : m_colour(colour), m_highlighted(false) {}

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
