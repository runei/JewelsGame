#include "Game.hpp"
#include "../external/loguru.hpp"
#include "../common/SDLUtils.hpp"

Game::Game() : m_isRunning(false), m_gameViewModel(Constants::GRID_ROWS, Constants::GRID_COLS), m_window(nullptr), m_renderer(nullptr), m_grid(&m_renderer, m_gameViewModel), m_scoreboard(&m_renderer, m_gameViewModel, m_grid), m_newGameButton(&m_renderer, "New Game"), m_exitButton(&m_renderer, "Exit") {

    // Set onClick functions for buttons
    m_newGameButton.setOnclickFunction([this]() {
        m_grid.resetGrid();
    });

    m_exitButton.setOnclickFunction([this]() {
        m_isRunning = false;
    });

    resetTimer();
}

Game::~Game() {
    m_grid.clearTextureCache();
    cleanup();
}

bool Game::initialize(const char* title, const int width, const int height) {
    try {
        // Initialize SDL
        SDLUtils::initializeSDL();

        // Create window and renderer
        m_window = SDLUtils::createWindow(title, width, height);
        m_renderer = SDLUtils::createRenderer(m_window);


        m_icon = IMG_Load("assets/images/Black.png");
        SDL_SetWindowIcon(m_window, m_icon);


        // Initialize frame time and game state
        m_prevFrameTime = SDL_GetTicks();
        m_isRunning = true;

        LOG_F(INFO, "Game initialized successfully.");
        return true;

    } catch (const SDLException& e) {
        LOG_F(ERROR, "%s: %s", e.what(), e.getSdlError());
        return false;
    }
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    // Handle various events based on event type
    switch (event.type) {
        case SDL_QUIT:
            handleQuitEvent();
            break;

        case SDL_MOUSEBUTTONDOWN:
            handleMouseButtonDownEvent(event);
            break;

        case SDL_MOUSEMOTION:
            handleMouseMotionEvent(event);
            break;

        case SDL_MOUSEBUTTONUP:
            handleMouseButtonUpEvent(event);
            break;

        default:
            break;
    }

    // Handle game over event for buttons
    if (m_gameViewModel.isGameOver()) {
        handleGameOverEvent(event);
    }
}

void Game::handleQuitEvent() {
    m_isRunning = false;
}

void Game::handleMouseButtonDownEvent(const SDL_Event& event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        m_grid.handleMouseClick(event.button.x, event.button.y);
        m_scoreboard.handleMouseClick(event);
    }
}

void Game::handleMouseMotionEvent(const SDL_Event& event) {
    m_grid.handleMouseMotion(event.motion.x, event.motion.y);
}

void Game::handleMouseButtonUpEvent(const SDL_Event& event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        m_grid.handleMouseRelease(event.button.x, event.button.y);
    }
}

void Game::handleGameOverEvent(const SDL_Event& event) {
    m_newGameButton.handleEvent(event);
    m_exitButton.handleEvent(event);
}


void Game::resetTimer(){
    m_collapseTimer = 100;// Constants::TIME_PER_FRAME;
}

void Game::update() {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 deltaTime = currentTime - m_prevFrameTime;

    // Update the collapse timer
    m_collapseTimer -= deltaTime;

    // If it's time to perform grid operations
    if (m_collapseTimer <= 0) {
        if (m_gameViewModel.fillEmptySpacesWithRandomColors() || m_gameViewModel.collapseEmptySpaces() ||
            m_gameViewModel.removeMatches() ||
            m_gameViewModel.rollbackSwap()) {

            m_grid.updateGrid();
        }

        // Reset the timer for the next cycle
        resetTimer();
    }

    // Update the previous frame time
    m_prevFrameTime = currentTime;
}


void Game::render() {
    SDL_RenderClear(m_renderer);

    if (m_gameViewModel.isGameOver()) {
        renderEndgameScreen();
    } else {
        m_grid.render();
        m_scoreboard.render();
    }

    SDL_RenderPresent(m_renderer);
}

void Game::renderEndgameScreen() {
    // Clear the renderer
    SDL_RenderClear(m_renderer);

    // Load the background image
    SDL_Texture* backgroundTexture = SDLUtils::loadImage(m_renderer, "assets/images/background.jpg");
    if (!backgroundTexture) {
        LOG_F(ERROR, "Error loading background image: %s", SDL_GetError());
        return;
    }

    // Render the background image
    SDL_Rect destRect = {0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT};
    SDL_RenderCopy(m_renderer, backgroundTexture, nullptr, &destRect);

    // Render the endgame screen UI
    SDL_Color textColor = {139, 0, 0};
    TTF_Font* font = TTF_OpenFont(SDLUtils::getFontPath(), 36);

    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, "Game Over", textColor);
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(m_renderer, gameOverSurface);

    // Calculate text position for "Game Over"
    int gameOverWidth = gameOverSurface->w;
    int gameOverHeight = gameOverSurface->h;
    int gameOverX = (Constants::SCREEN_WIDTH - gameOverWidth) / 2;
    int gameOverY = (Constants::SCREEN_HEIGHT - gameOverHeight) / 2 - 100;

    // Render "Game Over" text
    SDL_Rect gameOverRect = {gameOverX, gameOverY, gameOverWidth, gameOverHeight};
    SDL_RenderCopy(m_renderer, gameOverTexture, nullptr, &gameOverRect);

    // Render final score
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, ("Final Score: " + std::to_string(m_gameViewModel.getScore())).c_str(), textColor);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(m_renderer, scoreSurface);

    // Calculate text position for final score
    int scoreWidth = scoreSurface->w;
    int scoreHeight = scoreSurface->h;
    int scoreX = (Constants::SCREEN_WIDTH - scoreWidth) / 2;
    int scoreY = gameOverY + gameOverHeight + 20;

    // Render final score text
    SDL_Rect scoreRect = {scoreX, scoreY, scoreWidth, scoreHeight};
    SDL_RenderCopy(m_renderer, scoreTexture, nullptr, &scoreRect);

    // Create and render "New Game" button
    m_newGameButton.setPositionAndSize(scoreX, scoreY + scoreHeight + 20, scoreWidth, 50);
    m_newGameButton.render();

    // Create and render "Exit" button
    m_exitButton.setPositionAndSize(scoreX, scoreY + scoreHeight + 80, scoreWidth, 50);
    m_exitButton.render();

    // Clean up surfaces and textures
    SDL_FreeSurface(gameOverSurface);
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(scoreTexture);
    TTF_CloseFont(font);

    // Clean up background texture
    SDL_DestroyTexture(backgroundTexture);

    // Update the screen
    SDL_RenderPresent(m_renderer);
}


void Game::cleanup() {

    SDLUtils::destroy(m_renderer);
    SDLUtils::destroy(m_window);
    SDLUtils::destroy(m_icon);

    SDL_Quit();

    LOG_F(INFO, "Resources cleaned up.");
}

bool Game::isRunning() {
    return m_isRunning;
}

