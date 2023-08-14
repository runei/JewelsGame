#ifndef COLOUR_HPP
#define COLOUR_HPP

#include <string>
#include <unordered_map>

/* The Colour class provides color type enumeration and mapping for game objects.
To add a new jewel type follow this steps:
	1: Add a new color in enum class Type
	2: Add a new entry in nameToTypeMap in Colour.cpp
	3: Add a new entry in the ColourConfig.json
*/
class Colour {
public:
	enum class Type {
	    Black,
	    White,
	    Pink,
	    Blue,
	    Orange,

	    Unknown  // Used for empty spaces
	};

	static Type GetColorTypeByName(const std::string& name);

};

#endif // COLOUR_HPP
