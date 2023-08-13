#include "Game.hpp"
#include <iostream>


/*TODO

Remember to add logs
use constructor and destructor to initialize and clean?
*/

Game::Game() :  m_backgroundTexture(nullptr) {}

Game::~Game() {}

bool Game::initialize(const char* title, const int width, const int height) {

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		m_isRunning = false;
		return false;
	}

	// Create SDL window and game components
    m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);


	initializeImages();


	m_isRunning = true;
	return true;

}

void Game::initializeImages() {

	SDL_Surface* backgroundSurface = IMG_Load("assets/images/Backdrop13.jpg");
    if (backgroundSurface) {
        m_backgroundTexture = SDL_CreateTextureFromSurface(m_renderer, backgroundSurface);
    std::cout << "Ima\n";
        SDL_FreeSurface(backgroundSurface);
    } else {
        std::cout << "Image Load Error\n";
    }
}

void Game::handleEvents() {

	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			m_isRunning = false;
			break;

		default:
			break;
	}
}

void Game::update() {

}

void Game::render() {
	SDL_RenderClear(m_renderer);

	renderBackground();

	SDL_RenderPresent(m_renderer);
}

void Game::cleanup() {
	SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();

    if (m_backgroundTexture) {
        SDL_DestroyTexture(m_backgroundTexture);
    }
}

bool Game::isRunning() {
	return m_isRunning;
}

void Game::renderBackground() {
    if (m_backgroundTexture) {
        SDL_RenderCopy(m_renderer, m_backgroundTexture, nullptr, nullptr);
    }
}
