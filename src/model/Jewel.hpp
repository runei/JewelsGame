#ifndef JEWEL_HPP
#define JEWEL_HPP

#include "Colour.hpp"

class Jewel {
public:
    Jewel();
    explicit Jewel(Colour colour);

    Colour getColour() const;
    void setColour(Colour colour);

    bool isHighlighted() const;
    void setHighlighted(bool highlighted);


private:
    Colour m_colour;
    bool m_highlighted;
};

#endif // JEWEL_HPP
