#include "Scoreboard.hpp"
#include "../common/SDLUtils.hpp"
#include "../external/loguru.hpp"

Scoreboard::Scoreboard(SDL_Renderer** renderer, GameViewModel& gameViewModel, Grid& grid)
    : m_renderer(renderer), m_gameViewModel(gameViewModel), m_grid(grid),
      m_newGameButton(m_renderer, "New Game", getXPos(1), getYPos(4), getWidth(2), getHeight(0.75)) {

    // Set the onclick function for the "New Game" button
    m_newGameButton.setOnclickFunction([this]() {
        m_grid.resetGrid();
    });
}

Scoreboard::~Scoreboard() {
    // Destructor
}

// Convert screen coordinates to X position
int Scoreboard::getXPos(const double offset) const {
    return Constants::JEWEL_SIZE * (Constants::GRID_ROWS + offset);
}

// Convert screen coordinates to Y position
int Scoreboard::getYPos(const double offset) const {
    return Constants::JEWEL_SIZE * offset;
}

// Convert screen coordinates to width
int Scoreboard::getWidth(const double offset) const {
    return Constants::JEWEL_SIZE * offset;
}

// Convert screen coordinates to height
int Scoreboard::getHeight(const double offset) const {
    return Constants::JEWEL_SIZE * offset;
}

// Render the scoreboard
void Scoreboard::render() {
    renderBackground();

    // Render "SCORE" label and score value
    write("SCORE", 1, 0.4);
    writeScore(1.05, 1.1);

    // Render "TIME" label and time remaining
    write("TIME", 1, 2.4);
    write(std::to_string(m_gameViewModel.getTimeRemaining()), 1.75, 3.1);

    // Render the "New Game" button
    m_newGameButton.render();
}

// Handle mouse clicks in the scoreboard area
void Scoreboard::handleMouseClick(const SDL_Event& event) {
    m_newGameButton.handleEvent(event);
}

// Render text on the scoreboard
void Scoreboard::write(const std::string& text, const double xpos, const double ypos) {
    TTF_Init();

    SDL_Color textColor = {255, 255, 255, 255}; // White color with full opacity

    TTF_Font* font = TTF_OpenFont(SDLUtils::getFontPath(), 30);
    if (!font) {
        LOG_F(ERROR, "Error opening font: %s", TTF_GetError());
        return;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (!textSurface) {
        LOG_F(ERROR, "Error rendering text surface: %s", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(*m_renderer, textSurface);
    if (!textTexture) {
        LOG_F(ERROR, "Error creating text texture: %s", SDL_GetError());
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect textRect = {getXPos(xpos), getYPos(ypos), textSurface->w, textSurface->h};
    SDL_RenderCopy(*m_renderer, textTexture, nullptr, &textRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}

// Render the score value on the scoreboard
void Scoreboard::writeScore(const double xpos, const double ypos) {
    int scoreValue = m_gameViewModel.getScore();
    std::string scoreText = std::to_string(scoreValue);

    const int scoreLength = 5;

    // Pad with leading zeros to achieve a total length of 5 characters
    while (scoreText.length() < scoreLength) {
        scoreText = "0" + scoreText;
    }

    // Add spaces between each digit for better visibility
    std::string formattedScoreText;
    for (char digit : scoreText) {
        formattedScoreText += digit;
        formattedScoreText += " ";
    }

    write(formattedScoreText, xpos, ypos);
}

// Render the background of the scoreboard
void Scoreboard::renderBackground() {
    // Load the background image
    SDL_Texture* backgroundTexture = SDLUtils::loadImage(*m_renderer, "assets/images/scoreboard.jpg");
    if (!backgroundTexture) {
        LOG_F(ERROR, "Error loading background image: %s", SDL_GetError());
        return;
    }

    // Set rendering blend mode for transparency
    SDL_SetRenderDrawBlendMode(*m_renderer, SDL_BLENDMODE_BLEND);

    // Render the main background image
    SDL_Rect mainBackgroundRect = {getXPos(0), getYPos(0), getWidth(4), Constants::SCREEN_HEIGHT};
    SDL_RenderCopy(*m_renderer, backgroundTexture, nullptr, &mainBackgroundRect);

    // Render a semi-transparent red overlay
    SDL_SetRenderDrawColor(*m_renderer, 255, 0, 0, 60);
    SDL_Rect redOverlayRect = {getXPos(0), getYPos(0), getWidth(4), Constants::SCREEN_HEIGHT};
    SDL_RenderFillRect(*m_renderer, &redOverlayRect);

    // Reset blend mode
    SDL_SetRenderDrawBlendMode(*m_renderer, SDL_BLENDMODE_NONE);

    // Render black background for score section
    SDL_SetRenderDrawColor(*m_renderer, 0, 0, 0, 0);
    SDL_Rect scoreBackgroundRect = {getXPos(1), getYPos(1), getWidth(2), getHeight(0.75)};
    SDL_RenderFillRect(*m_renderer, &scoreBackgroundRect);

    // Render black background for time section
    SDL_SetRenderDrawColor(*m_renderer, 0, 0, 0, 0);
    SDL_Rect timeBackgroundRect = {getXPos(1), getYPos(3), getWidth(2), getHeight(0.75)};
    SDL_RenderFillRect(*m_renderer, &timeBackgroundRect);

    // Clean up background texture
    SDL_DestroyTexture(backgroundTexture);
}
