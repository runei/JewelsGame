#include "SDLUtils.hpp"
#include "../external/loguru.hpp"

void SDLUtils::initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    	throw SDLException("SDL initialization failed", SDL_GetError());
    }
}

SDL_Window* SDLUtils::createWindow(const char* title, const int width, const int height) noexcept(false) {

	SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

    if (!window) {
        throw SDLException("SDL window creation failed", SDL_GetError());
    }

    return window;

}

SDL_Renderer* SDLUtils::createRenderer(SDL_Window* window) noexcept(false) {

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        throw SDLException("SDL renderer creation failed", SDL_GetError());
    }

    return renderer;
}

SDL_Texture* SDLUtils::loadImage(SDL_Renderer* renderer, const std::string& imagePath) noexcept(false) {

    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
    	const std::string err_msg = "Error loading image: " + imagePath;
    	throw SDLException(err_msg);
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
    	throw SDLException("Failed to create texture from surface", SDL_GetError());
    }

    return texture;
}

void SDLUtils::destroy(SDL_Texture* texture) {
	if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void SDLUtils::destroy(SDL_Renderer* renderer) {
	if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
}

void SDLUtils::destroy(SDL_Window* window) {
	if (window) {
        SDL_DestroyWindow(window);
    }
}
