#ifndef JEWEL_HPP
#define JEWEL_HPP

#include "../common/Constants.hpp"
#include "Colour.hpp"

class Jewel {
public:
    Jewel();
    Jewel(Colour colour);

    Colour getColour() const;

private:
    Colour m_colour;
};

#endif // JEWEL_HPP
