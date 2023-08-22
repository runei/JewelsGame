#include "Button.hpp"
#include "../common/SDLUtils.hpp"

#include "../external/loguru.hpp"

// Constructor with parameters to set initial position and size
Button::Button(SDL_Renderer** renderer, const char* buttonText, int x, int y, int width, int height)
    : m_renderer(renderer), m_x(x), m_y(y), m_width(width), m_height(height), m_text(buttonText) {

    m_rect = {x, y, width, height};
}

// Constructor with only renderer and text
Button::Button(SDL_Renderer** renderer, const char* buttonText)
    : m_renderer(renderer), m_text(buttonText) {
}

// Default constructor
Button::Button() {
}

// Destructor
Button::~Button() {
}

// Render the button onto the renderer
void Button::render() {
    // Create a texture for the button using SDLUtils
    SDL_Texture* texture = SDLUtils::createButtonTexture(*m_renderer, m_text, m_x, m_y, m_width, m_height);

    // Render the texture onto the renderer
    SDL_RenderCopy(*m_renderer, texture, nullptr, &m_rect);

    SDL_DestroyTexture(texture);
}

// Handle mouse events on the button
bool Button::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        // Check if the mouse is within the button's bounds
        if (mouseX >= m_rect.x && mouseX < m_rect.x + m_rect.w &&
            mouseY >= m_rect.y && mouseY < m_rect.y + m_rect.h) {
            if (m_onclickCallback) {
                m_onclickCallback();
                return true;
            }
        }
    }
    return false;
}

// Set the callback function to be executed on button click
void Button::setOnclickFunction(std::function<void()> callback) {
    m_onclickCallback = callback;
}

// Set the button's position and size
void Button::setPositionAndSize(int x, int y, int width, int height) {
    m_x = x;
    m_y = y;
    m_height = height;
    m_width = width;

    m_rect = {x, y, width, height};
}
