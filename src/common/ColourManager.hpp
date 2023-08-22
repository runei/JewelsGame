#ifndef COLOUR_MANAGER_HPP
#define COLOUR_MANAGER_HPP

#include <unordered_map>
#include <string>
#include "../external/json.hpp"
#include "../model/Colour.hpp"


// Class for managing colours
class ColourManager {
public:
    // Constructor and Destructor
    ColourManager();
    ~ColourManager();

    // Get the image path for a given colour
    std::string getImgPathForColour(const Colour& colour) const;

    // Get a random colour
    Colour getRandomColour() const;

private:
    // Mapping of colours to image paths
    std::unordered_map<Colour, std::string> m_colourImgMap;

    // Get a Colour based on its name
    Colour getColourByName(const std::string& name);

    // Load colours from a configuration
    bool loadColoursFromConfig();
};

#endif // COLOUR_MANAGER_HPP
