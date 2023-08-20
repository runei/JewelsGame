#include "Button.hpp"
#include "../common/SDLUtils.hpp"

Button::Button(SDL_Renderer** renderer, const char* buttonText, int x, int y, int width, int height)
    : m_renderer(renderer), m_x(x), m_y(y), m_height(height), m_width(width), m_text(buttonText) {

    m_rect = {x, y, width, height};
}

Button::~Button() {
    SDL_DestroyTexture(m_texture);
}

void Button::render() {
    SDL_Texture* texture = SDLUtils::createButtonTexture(*m_renderer, m_text, m_x, m_y, m_width, m_height);

    SDL_RenderCopy(*m_renderer, texture, nullptr, &m_rect);
}

bool Button::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

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

void Button::setOnclickFunction(std::function<void()> callback) {
    m_onclickCallback = callback;
}
