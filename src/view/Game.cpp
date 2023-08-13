#include "Game.hpp"
#include "../external/loguru.hpp"


/*TODO

Remember to add logs
use constructor and destructor to initialize and clean?
*/

Game::Game() :  m_backgroundTexture(nullptr), m_isRunning(false) {}

Game::~Game() {}

bool Game::initialize(const char* title, const int width, const int height) {
    if (!initializeSDL()) {
        return false;
    }

    if (!createWindowAndRenderer(title, width, height)) {
    	SDL_Quit();
        return false;
    }

    initializeImages();

    m_prevFrameTime = SDL_GetTicks();
    m_isRunning = true;

    LOG_F(INFO, "Game initialized successfully.");
    return true;
}

bool Game::initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        LOG_F(ERROR, "SDL initialization failed: %s", SDL_GetError());
        return false;
    }
    LOG_F(INFO, "SDL initialized successfully.");
    return true;
}

bool Game::createWindowAndRenderer(const char* title, const int width, const int height) {

    m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!m_window) {
        LOG_F(ERROR, "SDL window creation failed: %s", SDL_GetError());
        return false;
    }
    LOG_F(INFO, "SDL window created successfully.");

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        LOG_F(ERROR, "SDL renderer creation failed: %s", SDL_GetError());
        return false;
    }

    LOG_F(INFO, "SDL renderer created successfully.");
    return true;
}

void Game::initializeImages() {
    LOG_F(INFO, "Initializing images...");

    SDL_Surface* backgroundSurface = IMG_Load("assets/images/Backdrop13.jpg");
    if (backgroundSurface) {
        LOG_F(INFO, "Loaded background image successfully.");

        m_backgroundTexture = SDL_CreateTextureFromSurface(m_renderer, backgroundSurface);
        SDL_FreeSurface(backgroundSurface);

        if (m_backgroundTexture) {
            LOG_F(INFO, "Background texture created successfully.");
        } else {
            LOG_F(ERROR, "Failed to create background texture: %s", SDL_GetError());
        }
    } else {
        LOG_F(ERROR, "Failed to load background image: %s", IMG_GetError());
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
	// Calculate delta time
	Uint32 currentTime = SDL_GetTicks();
	Uint32 deltaTime = currentTime - m_prevFrameTime;

	// Update game logic based on deltaTime
	// ...

	m_prevFrameTime = currentTime;

	// Delay to achieve desired frame rate
	delayIfNeeded();

}

void Game::render() {
	SDL_RenderClear(m_renderer);

	renderBackground();

	SDL_RenderPresent(m_renderer);
}

void Game::cleanup() {
    LOG_F(INFO, "Cleaning up resources...");

    if (m_backgroundTexture) {
        SDL_DestroyTexture(m_backgroundTexture);
        LOG_F(INFO, "Background texture destroyed.");
    }

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();

    LOG_F(INFO, "Resources cleaned up.");
}


bool Game::isRunning() {
	return m_isRunning;
}

void Game::renderBackground() {
	if (m_backgroundTexture) {
		SDL_RenderCopy(m_renderer, m_backgroundTexture, nullptr, nullptr);
	}
}

void Game::delayIfNeeded() {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsedTime = currentTime - m_prevFrameTime;

    if (elapsedTime < TIME_PER_FRAME) {
        Uint32 delayTime = TIME_PER_FRAME - elapsedTime;
        // LOG_F(INFO, "Delaying for %u ms to achieve desired frame rate.", delayTime);
        SDL_Delay(delayTime);
    }

    m_prevFrameTime = SDL_GetTicks();
}
