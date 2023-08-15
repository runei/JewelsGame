#ifndef JEWEL_HPP
#define JEWEL_HPP

#include "Colour.hpp"

class Jewel {
public:
    Jewel();
    explicit Jewel(Colour colour);

    Colour getColour() const;
    void setColour(Colour colour);

private:
    Colour m_colour;
};

#endif // JEWEL_HPP
