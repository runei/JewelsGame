#ifndef SDL_UTILS_HPP
#define SDL_UTILS_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "../common/Constants.hpp"
#include <string>
#include "SDLException.hpp"

namespace SDLUtils {

// Initialize the SDL library.
// throws SDLException if SDL initialization fails.
void initializeSDL();

// Create an SDL window.
SDL_Window* createWindow(const char* title, const int width, const int height);

// Create an SDL renderer for a given window.
SDL_Renderer* createRenderer(SDL_Window* window);

// Load an image into an SDL texture.
SDL_Texture* loadImage(SDL_Renderer* renderer, const std::string& imagePath);

// Get the path to the font file.
const char* getFontPath();

// Create an SDL texture for a button with text.
SDL_Texture* createButtonTexture(SDL_Renderer* renderer, const std::string& buttonText, int x, int y, int width, int height);

void destroy(SDL_Texture* texture);
void destroy(SDL_Renderer* renderer);
void destroy(SDL_Window* window);

} // namespace SDLUtils

#endif // SDL_UTILS_HPP
