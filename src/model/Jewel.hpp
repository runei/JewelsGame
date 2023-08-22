#ifndef JEWEL_HPP
#define JEWEL_HPP

#include "Colour.hpp"

class Jewel {
public:
    // Default constructor
    Jewel();

    // Constructor with colour assignment
    Jewel(Colour colour);

    // Getter and setter for colour
    Colour getColour() const;
    void setColour(Colour colour);

    // Getter and setter for highlighting
    bool isHighlighted() const;
    void setHighlighted(bool highlighted);

private:
    Colour m_colour;
    bool m_highlighted;
};

#endif // JEWEL_HPP
