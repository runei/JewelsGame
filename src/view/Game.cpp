#include "Game.hpp"

Game::Game() {}

Game::~Game() {}

bool Game::initialize(const char* title, const int xpos, const int ypos, const int width, const int height) {

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) == 0) {

		// Create SDL window and game components
	    m_window = SDL_CreateWindow(title, xpos, ypos, width, height, 0);
		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

		m_isRunning = true;
		return true;
	} else {
		m_isRunning = false;
		return false;
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

}

void Game::cleanup() {
	SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool Game::isRunning() {
	return m_isRunning;
}
