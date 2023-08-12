#ifndef SDL_RENDERER_HPP
#define SDL_RENDERER_HPP

#include "SDL2/SDL.h"

class SDLRenderer {
public:
    SDLRenderer(SDL_Renderer* renderer);
    ~SDLRenderer();

    void Initialize();
    void Clear();
    void RenderTexture(SDL_Texture* texture, int x, int y, int width, int height);
    void Present();

    void Cleanup();

private:
    SDL_Renderer* m_renderer;
};

#endif // SDL_RENDERER_HPP
