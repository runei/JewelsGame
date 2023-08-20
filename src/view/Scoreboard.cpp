#include "Scoreboard.hpp"
#include "../common/SDLUtils.hpp"
#include "../external/loguru.hpp"

Scoreboard::Scoreboard(SDL_Renderer** renderer, GameViewModel& gameViewModel)
    : m_renderer(renderer), m_gameViewModel(gameViewModel) {

    TTF_Init();
    m_font = SDLUtils::getFont();

}

Scoreboard::~Scoreboard() {

}

int Scoreboard::getXPos(const double offset) const {
	return Constants::JEWEL_SIZE * (Constants::GRID_ROWS + offset);
}

int Scoreboard::getYPos(const double offset) const {
	return Constants::JEWEL_SIZE * offset;
}

void Scoreboard::render() {

	renderBackground();

	write("SCORE:", 1, 1.4);
    writeScore(1.05, 2.1);

    write("TIME:", 1, 3.4);
    write(std::to_string(m_gameViewModel.getTimeRemaining()), 1.75, 4.1);
}

void Scoreboard::write(const std::string& text, const double xpos, const double ypos) {

		// Draw the "SCORE" text
    SDL_Color textColor = {255, 255, 255, 255}; // White color with full opacity

    SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(*m_renderer, textSurface);

    SDL_Rect textRect = {getXPos(xpos), getYPos(ypos), textSurface->w, textSurface->h}; // Position (100, 50)
    SDL_RenderCopy(*m_renderer, textTexture, nullptr, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

}

void Scoreboard::writeScore(const double xpos, const double ypos) {

    std::string scoreText;
    std::string scoreValueStr = std::to_string(m_gameViewModel.getScore());


    const int scoreLength = 5;

    // Add leading zeros to make the total length 4 characters
    while (scoreValueStr.length() < scoreLength) {
        scoreValueStr = "0" + scoreValueStr;
    }

    for (int i = 0; i < scoreLength; ++i) {
        scoreText += scoreValueStr[i];
        scoreText += " ";
    }

    write(scoreText, xpos, ypos);
}

void Scoreboard::renderBackground() {

	// Set rendering blend mode for transparency
    SDL_SetRenderDrawBlendMode(*m_renderer, SDL_BLENDMODE_BLEND);

    // Draw the semi-transparent red rectangle
    SDL_SetRenderDrawColor(*m_renderer, 255, 0, 0, 128);
    SDL_Rect rect = {getXPos(0), getYPos(0), Constants::JEWEL_SIZE * 4, Constants::SCREEN_HEIGHT};
    SDL_RenderFillRect(*m_renderer, &rect);


    SDL_SetRenderDrawBlendMode(*m_renderer, SDL_BLENDMODE_NONE);


    // Draw the score background black
    SDL_SetRenderDrawColor(*m_renderer, 0, 0, 0, 0);
    SDL_Rect rect2 = {getXPos(1), getYPos(2), Constants::JEWEL_SIZE * 2, int(Constants::JEWEL_SIZE * 0.75) };
    SDL_RenderFillRect(*m_renderer, &rect2);


    // Draw the time background black
    SDL_SetRenderDrawColor(*m_renderer, 0, 0, 0, 0);
    SDL_Rect rect3 = {getXPos(1), getYPos(4), Constants::JEWEL_SIZE * 2, int(Constants::JEWEL_SIZE * 0.75) };
    SDL_RenderFillRect(*m_renderer, &rect3);

}
