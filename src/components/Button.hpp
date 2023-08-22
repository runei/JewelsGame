#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "SDL2/SDL.h"
#include <functional>
#include <memory>

class Button {
public:
    Button();

    // Constructor with only renderer and text
    Button(SDL_Renderer** renderer, const char* buttonText);

    // Constructor with parameters to set initial position and size
    Button(SDL_Renderer** renderer, const char* buttonText, int x, int y, int width, int height);

    ~Button();

    // Render the button onto the renderer
    void render();

    // Handle mouse events on the button
    bool handleEvent(const SDL_Event& event);

    // Set the callback function to be executed on button click
    void setOnclickFunction(std::function<void()> callback);

    // Set the button's position and size
    void setPositionAndSize(int x, int y, int width, int height);

private:
    SDL_Renderer** m_renderer;
    SDL_Rect m_rect;
    std::function<void()> m_onclickCallback;

    int m_x;
    int m_y;
    int m_width;
    int m_height;
    const char* m_text;
};

#endif // BUTTON_HPP
