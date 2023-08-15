#ifndef COLOUR_MANAGER_HPP
#define COLOUR_MANAGER_HPP

#include "../external/json.hpp"

#include <unordered_map>
#include <string>

#include "../model/Colour.hpp"

class ColourManager {
public:
	ColourManager();
	~ColourManager();

	std::string getImgPathForColour(const Colour& colour) const;

private:

	std::unordered_map<Colour, std::string> m_colourImgMap;

	Colour getColourByName(const std::string& name);

	bool loadColoursFromConfig();
	// SDL_Texture* loadTexture(const std::string& imagePath);

};

#endif // COLOUR_MANAGER_HPP
