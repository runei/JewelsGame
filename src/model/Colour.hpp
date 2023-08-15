#ifndef COLOUR_HPP
#define COLOUR_HPP

/* The Colour class provides color type enumeration and mapping for game objects.
To add a new jewel type follow this steps:
	1: Add a new color in enum class Type
	2: Add a new entry in nameToTypeMap in ColourManager.cpp
	3: Add a new entry in the ColourConfig.json
*/
enum class Colour {
    Black,
    White,
    Pink,
    Blue,
    Orange,

    Unknown  // Used for empty spaces
};

#endif // COLOUR_HPP
