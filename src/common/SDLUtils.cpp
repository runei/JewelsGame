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

const char* SDLUtils::getFontPath() {

    return "assets/fonts/OldLondon.ttf";
}

SDL_Texture* SDLUtils::createButtonTexture(SDL_Renderer* renderer, const std::string& buttonText, int x, int y, int width, int height) {

    TTF_Init();

    // Create a surface with grey background
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 100, 100, 100, 255)); // Grey color

    // Create a font and render the button text
    TTF_Font* font = TTF_OpenFont(getFontPath(), 28);
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, buttonText.c_str(), textColor);

    // Calculate text position to center it on the button
    int textX = (width - textSurface->w) / 2;
    int textY = (height - textSurface->h) / 2;

    // Blit the text surface onto the button surface
    SDL_Rect textRect = {textX, textY, textSurface->w, textSurface->h};
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);

    // Create a texture from the button surface
    SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, surface);

    // Clean up surfaces and fonts
    TTF_CloseFont(font);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(textSurface);

    return buttonTexture;
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
