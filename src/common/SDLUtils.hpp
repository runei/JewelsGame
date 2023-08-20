#ifndef SDL_UTILS_HPP
#define SDL_UTILS_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDLException.hpp"
#include <string>

namespace SDLUtils {

	void initializeSDL() noexcept(false);

	SDL_Window* createWindow(const char* title, const int width, const int height) noexcept(false);

	SDL_Renderer* createRenderer(SDL_Window* window) noexcept(false);

	SDL_Texture* loadImage(SDL_Renderer* renderer, const std::string& imagePath) noexcept(false);

	const char* getFontPath();

	SDL_Texture* createButtonTexture(SDL_Renderer* renderer, const std::string& buttonText, int x, int y, int width, int height);

	void destroy(SDL_Texture* texture);
	void destroy(SDL_Renderer* renderer);
	void destroy(SDL_Window* window);

}

#endif // SDL_UTILS_HPP
