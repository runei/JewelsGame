#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "SDL2/SDL.h"
#include <functional>

class Button {
public:
    Button();
    Button(SDL_Renderer** renderer, const char* buttonText);
    Button(SDL_Renderer** renderer, const char* buttonText, int x, int y, int width, int height);
    ~Button();

    void render();
    bool handleEvent(const SDL_Event& event);
    void setOnclickFunction(std::function<void()> callback);
    void setPositionAndSize(int x, int y, int width, int height);

private:
    SDL_Renderer** m_renderer;
    SDL_Texture* m_texture;
    SDL_Rect m_rect;
    std::function<void()> m_onclickCallback;

    int m_x;
    int m_y;
    int m_width;
    int m_height;
    const char* m_text;
};

#endif // BUTTON_HPP
