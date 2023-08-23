#include "SDLUtils.hpp"
#include "../external/loguru.hpp"
#include <memory>

// Initialize the SDL library.
// throws SDLException if SDL initialization fails.
void SDLUtils::initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    	throw SDLException("SDL initialization failed", SDL_GetError());
    }


    TTF_Init();

}

// Create an SDL window.
SDL_Window* SDLUtils::createWindow(const char* title, const int width, const int height) noexcept(false) {

    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

    if (!window) {
        throw SDLException("Failed to create SDL window", SDL_GetError());
    }

    return window;
}

// Create an SDL renderer for a given window.
SDL_Renderer* SDLUtils::createRenderer(SDL_Window* window) noexcept(false) {

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        throw SDLException("Failed to create SDL renderer", SDL_GetError());
    }

    return renderer;
}

// Load an image into an SDL texture.
SDL_Texture* SDLUtils::loadImage(SDL_Renderer* renderer, const std::string& imagePath) noexcept(false) {

    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface(IMG_Load(imagePath.c_str()), &SDL_FreeSurface);

    if (!surface) {
        throw SDLException("Error loading image: " + imagePath, IMG_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface.get());

    if (!texture) {
        throw SDLException("Failed to create texture from surface", SDL_GetError());
    }

    return texture;
}

// Get the path to the font file.
const char* SDLUtils::getFontPath() {

    return "assets/fonts/Seagram.ttf";
}

// Create an SDL texture for a button with text.
SDL_Texture* SDLUtils::createButtonTexture(SDL_Renderer* renderer, const std::string& buttonText, int x, int y, int width, int height) {

    TTF_Font* font = TTF_OpenFont(getFontPath(), Constants::BUTTON_FONT_SIZE);
    if (!font) {
        LOG_F(ERROR, "Failed to load font: %s", TTF_GetError());
        return nullptr;
    }

    // Create a surface with grey background
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface(SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32), &SDL_FreeSurface);
    if (!surface) {
        TTF_CloseFont(font);
        return nullptr;
    }
    SDL_FillRect(surface.get(), nullptr, SDL_MapRGBA(surface->format, 200, 200, 200, 255)); // Grey color

    // Render the button text to a surface
    SDL_Color textColor = {139, 0, 0, 255};
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> textSurface(TTF_RenderText_Solid(font, buttonText.c_str(), textColor), &SDL_FreeSurface);
    if (!textSurface) {
        TTF_CloseFont(font);
        return nullptr;
    }

    // Calculate text position to center it on the button
    int textX = (width - textSurface->w) / 2;
    int textY = (height - textSurface->h) / 2;

    // Blit the text surface onto the button surface
    SDL_Rect textRect = {textX, textY, textSurface->w, textSurface->h};
    SDL_BlitSurface(textSurface.get(), nullptr, surface.get(), &textRect);

    // Create a texture from the button surface
    SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, surface.get());

    TTF_CloseFont(font);
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

void SDLUtils::destroy(SDL_Surface* surface) {
    if (surface) {
        SDL_FreeSurface(surface);
    }
}
