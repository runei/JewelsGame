#include "Game.hpp"
#include "../external/loguru.hpp"

Game::Game() : m_isRunning(false), m_gameViewModel(Constants::GRID_ROWS, Constants::GRID_COLS), m_grid(m_gameViewModel), m_collapseTimer(Constants::GRID_COLLAPSE_DELAY) {}

Game::~Game() {
    m_grid.clearTextureCache();

    cleanup();
}

bool Game::initialize(const char* title, const int width, const int height) {
    if (!initializeSDL()) {
        return false;
    }

    if (!createWindowAndRenderer(title, width, height)) {
        cleanup();
        return false;
    }

    if (!initializeResources()) {
        cleanup();
        return false;
    }

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

bool Game::initializeResources() {
    LOG_F(INFO, "Initializing resources...");

    m_backgroundTexture = loadTexture("assets/images/Backdrop13.jpg");
    if (!m_backgroundTexture) {
        cleanup();
        return false;
    }

    LOG_F(INFO, "Resources initialized successfully.");
    return true;
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            m_isRunning = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                m_grid.handleMouseClick(event.button.x, event.button.y, m_renderer);
            }
            break;

        case SDL_MOUSEMOTION:
            m_grid.handleMouseMotion(event.motion.x, event.motion.y);
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                m_grid.handleMouseRelease(event.button.x, event.button.y, m_renderer);
            }
            break;

        default:
            break;
    }
}

void Game::resetTimer(){
    m_collapseTimer = Constants::GRID_COLLAPSE_DELAY;
}

void Game::updateCollapseAndFill(const double deltaTime) {
    bool jewelsRemoved = m_gameViewModel.removeMatches();

    // Reset the timer when jewels are removed
    if (jewelsRemoved) {
        m_grid.updateGrid();
        resetTimer();
    }

    // Update the collapse timer
    m_collapseTimer -= deltaTime;

    // Perform collapse when the timer reaches zero
    if (m_collapseTimer <= 0) {
        bool collapsed = m_gameViewModel.collapseEmptySpaces();
        if (collapsed) {
            m_grid.updateGrid();
            resetTimer();

            // m_gameViewModel.fillEmptySpacesWithRandomColors();
        }
    }
}

void Game::update() {


    Uint32 currentTime = SDL_GetTicks();
    Uint32 deltaTime = currentTime - m_prevFrameTime;

    updateCollapseAndFill(deltaTime);

    m_prevFrameTime = currentTime;

    delayIfNeeded();
}

void Game::render() {
    SDL_RenderClear(m_renderer);

    renderBackground();
    m_grid.render(m_renderer);

    SDL_RenderPresent(m_renderer);
}

void Game::cleanup() {
    if (m_backgroundTexture) {
        SDL_DestroyTexture(m_backgroundTexture);
        LOG_F(INFO, "Background texture destroyed.");
        m_backgroundTexture = nullptr;
    }

    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }

    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

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

SDL_Texture* Game::loadTexture(const std::string& imagePath) {
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        LOG_F(ERROR, "Error loading image: %s", imagePath.c_str());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        LOG_F(ERROR, "Failed to create texture from surface: %s", SDL_GetError());
    }

    return texture;
}
