#include "Game.hpp"
#include "../external/loguru.hpp"
#include "../common/SDLUtils.hpp"

Game::Game() : m_isRunning(false), m_gameViewModel(Constants::GRID_ROWS, Constants::GRID_COLS), m_window(nullptr), m_renderer(nullptr), m_backgroundTexture(nullptr), m_grid(&m_renderer, m_gameViewModel), m_scoreboard(&m_renderer, m_gameViewModel, m_grid), m_newGameButton(&m_renderer, "New Game"), m_exitButton(&m_renderer, "Exit") {


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

        SDLUtils::initializeSDL();

        m_window = SDLUtils::createWindow(title, width, height);

        m_renderer = SDLUtils::createRenderer(m_window);

        m_backgroundTexture = SDLUtils::loadImage(m_renderer, "assets/images/Backdrop13.jpg");

    } catch (const SDLException& e) {

        LOG_F(ERROR, "%s: %s", e.what(), e.getSdlError());
        return false;

    }

    m_prevFrameTime = SDL_GetTicks();
    m_isRunning = true;

    LOG_F(INFO, "Game initialized successfully.");
    return true;
}

bool Game::initializeResources() {
    LOG_F(INFO, "Initializing resources...");


    LOG_F(INFO, "Resources initialized successfully.");
    return true;
}

void Game::handleEvents() {


    SDL_Event event;
    SDL_PollEvent(&event);


    if (event.type == SDL_QUIT) {
        m_isRunning = false;
        return;
    }


    if (m_gameViewModel.isGameOver()) {
        m_newGameButton.handleEvent(event);
        m_exitButton.handleEvent(event);
        return;
    }

    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                m_grid.handleMouseClick(event.button.x, event.button.y);
                m_scoreboard.handleMouseClick(event);
            }
            break;

        case SDL_MOUSEMOTION:
            m_grid.handleMouseMotion(event.motion.x, event.motion.y);
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                m_grid.handleMouseRelease(event.button.x, event.button.y);
            }
            break;

        default:
            break;
    }
}

void Game::resetTimer(){
    m_collapseTimer = 100;// Constants::TIME_PER_FRAME;
}

void Game::update() {

    Uint32 currentTime = SDL_GetTicks();
    Uint32 deltaTime = currentTime - m_prevFrameTime;

    m_collapseTimer -= deltaTime;

    if (m_collapseTimer <= 0) {

        if (m_gameViewModel.fillEmptySpacesWithRandomColors()) {
            m_grid.updateGrid();
        } else if (m_gameViewModel.collapseEmptySpaces()) {
            m_grid.updateGrid();
        } else if (m_gameViewModel.removeMatches()) {
            m_grid.updateGrid();
        } else if (m_gameViewModel.rollbackSwap()) {
            m_grid.updateGrid();
        }
        resetTimer();
    }

    m_prevFrameTime = currentTime;

    // delayIfNeeded();
}

void Game::render() {
    SDL_RenderClear(m_renderer);

    if (m_gameViewModel.isGameOver()) {
        renderEndgameScreen();
    } else {
        renderBackground();
        m_grid.render();
        m_scoreboard.render();
    }

    SDL_RenderPresent(m_renderer);
}

void Game::renderEndgameScreen() {
    // Clear the renderer
    SDL_RenderClear(m_renderer);

    // Render the endgame screen UI
    SDL_Color textColor = {255, 255, 255}; // White text color
    TTF_Font* font = TTF_OpenFont(SDLUtils::getFontPath(), 36);

    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, "Game Over", textColor);
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(m_renderer, gameOverSurface);

    // Calculate text position for "Game Over"
    int gameOverWidth = gameOverSurface->w;
    int gameOverHeight = gameOverSurface->h;
    int gameOverX = (Constants::SCREEN_WIDTH - gameOverWidth) / 2;
    int gameOverY = (Constants::SCREEN_HEIGHT - gameOverHeight) / 2 - 50;

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

    // Update the screen
    SDL_RenderPresent(m_renderer);
}

void Game::cleanup() {

    SDLUtils::destroy(m_backgroundTexture);
    SDLUtils::destroy(m_renderer);
    SDLUtils::destroy(m_window);

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

    if (elapsedTime < Constants::TIME_PER_FRAME) {
        Uint32 delayTime = Constants::TIME_PER_FRAME - elapsedTime;
        SDL_Delay(delayTime);
    }

    m_prevFrameTime = SDL_GetTicks();
}
