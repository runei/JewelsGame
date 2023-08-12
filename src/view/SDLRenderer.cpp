#include "SDLRenderer.hpp"

SDLRenderer::SDLRenderer(SDL_Renderer* renderer) : m_renderer(renderer) {}

SDLRenderer::~SDLRenderer() {
    Cleanup();
}

void SDLRenderer::Initialize() {
    // Initialize renderer settings, if needed
}

void SDLRenderer::Clear() {
    SDL_RenderClear(m_renderer);
}

void SDLRenderer::RenderTexture(SDL_Texture* texture, int x, int y, int width, int height) {
    SDL_Rect destRect = { x, y, width, height };
    SDL_RenderCopy(m_renderer, texture, nullptr, &destRect);
}

void SDLRenderer::Present() {
    SDL_RenderPresent(m_renderer);
}

void SDLRenderer::Cleanup() {
    // Clean up resources, if needed
}

