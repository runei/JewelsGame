#include "Grid.hpp"
#include "../external/loguru.hpp"
#include "../common/SDLUtils.hpp"

Grid::Grid(SDL_Renderer** renderer, GameViewModel& viewModel)
    : m_renderer(renderer), m_viewModel(viewModel), m_gridTexture(nullptr), m_dragging(false), m_dragJewelTexture(nullptr) {
}

Grid::~Grid() {
    SDLUtils::destroy(m_gridTexture);
    SDLUtils::destroy(m_dragJewelTexture);

    clearTextureCache();
}

SDL_Texture* Grid::getOrCreateTexture(const std::string& imagePath) {

    if (m_textureCache.count(imagePath)) {
        return m_textureCache[imagePath];
    }

    SDL_Texture* texture = nullptr;

    try {

        texture = SDLUtils::loadImage(*m_renderer, imagePath);

        m_textureCache[imagePath] = texture;

    } catch (const SDLException& e) {

        LOG_F(ERROR, "%s: %s", e.what(), e.getSdlError());

    }

    return texture;
}

void Grid::render() {
    if (!m_gridTexture) {
        createGridTexture();
    }

    // Set the rendering viewport to match the grid size
    SDL_Rect viewport = {0, 0, m_viewModel.getNumCols() * Constants::JEWEL_SIZE, m_viewModel.getNumRows() * Constants::JEWEL_SIZE};
    SDL_RenderSetViewport(*m_renderer, &viewport);

    // Render the grid texture
    SDL_RenderCopy(*m_renderer, m_gridTexture, nullptr, nullptr);

    // Reset the rendering viewport
    SDL_RenderSetViewport(*m_renderer, nullptr);

    if (m_dragging) {
        if (m_dragJewelTexture) {
            int destX = m_dragDest.first - Constants::JEWEL_SIZE / 2;
            int destY = m_dragDest.second - Constants::JEWEL_SIZE / 2;

            SDL_Rect destRect = {destX, destY, Constants::JEWEL_SIZE, Constants::JEWEL_SIZE};
            SDL_RenderCopy(*m_renderer, m_dragJewelTexture, nullptr, &destRect);
        }
    }
}

void Grid::createGridTexture() {
    m_gridTexture = SDL_CreateTexture(*m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_viewModel.getNumCols() * Constants::JEWEL_SIZE, m_viewModel.getNumRows() * Constants::JEWEL_SIZE);


    if (!m_gridTexture) {
        LOG_F(ERROR, "Error creating grid texture: %s", SDL_GetError());
        return;
    }

    SDL_SetRenderTarget(*m_renderer, m_gridTexture);
    SDL_SetRenderDrawColor(*m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(*m_renderer);

    for (int row = 0; row < m_viewModel.getNumRows(); ++row) {
        for (int col = 0; col < m_viewModel.getNumCols(); ++col) {
            std::string imagePath = m_viewModel.getColourImgPath(row, col);
            if (!imagePath.empty()) {
                SDL_Texture* jewelTexture = getOrCreateTexture(imagePath);
                if (jewelTexture) {
                    int destX = col * Constants::JEWEL_SIZE;
                    int destY = row * Constants::JEWEL_SIZE;

                    if (m_viewModel.isJewelHighlighted(row, col)) {
                        SDL_SetRenderDrawColor(*m_renderer, 255, 255, 0, 255);
                        SDL_Rect highlightRect = {destX, destY, Constants::JEWEL_SIZE, Constants::JEWEL_SIZE};
                        SDL_RenderDrawRect(*m_renderer, &highlightRect);
                        SDL_SetRenderDrawColor(*m_renderer, 0, 0, 0, 255);
                    }

                    SDL_Rect destRect = {destX, destY, Constants::JEWEL_SIZE, Constants::JEWEL_SIZE};
                    SDL_RenderCopy(*m_renderer, jewelTexture, nullptr, &destRect);
                }
            }
        }
    }

    SDL_SetRenderTarget(*m_renderer, nullptr);
}

void Grid::handleMouseClick(int x, int y) {
    int row = y / Constants::JEWEL_SIZE;
    int col = x / Constants::JEWEL_SIZE;

    if (row >= 0 && row < m_viewModel.getNumRows() && col >= 0 && col < m_viewModel.getNumCols()) {
        if (m_viewModel.toggleJewelHighlight(row, col)) {

            m_dragging = true;
            m_dragStart = std::make_pair(row, col);
            m_dragDest.first = col * Constants::JEWEL_SIZE + Constants::JEWEL_SIZE / 2;
            m_dragDest.second = row * Constants::JEWEL_SIZE + Constants::JEWEL_SIZE / 2;

            // Load the jewel texture for dragging
            std::string imagePath = m_viewModel.getColourImgPath(m_dragStart.first, m_dragStart.second);
            m_dragJewelTexture = getOrCreateTexture(imagePath);

            // Update the grid and render
            updateGrid();

            LOG_F(INFO, "Grid texture reset.");
        }
    }
}


void Grid::handleMouseMotion(int x, int y) {
    if (m_dragging) {
        // Update destination position during drag movement
        m_dragDest = std::make_pair(x, y);
    }
}

void Grid::handleMouseRelease(int x, int y) {
    if (m_dragging) {

        const int col = x / Constants::JEWEL_SIZE;
        const int row = y / Constants::JEWEL_SIZE;

        if ((row == m_dragStart.first && abs(col - m_dragStart.second) == 1) || (col == m_dragStart.second && abs(row - m_dragStart.first) == 1)) {

            m_viewModel.swapJewels(std::make_pair(row, col), m_dragStart);

            m_gridTexture = nullptr;

            LOG_F(INFO, "Grid texture release reset.");
        }

        m_dragging = false;
        m_dragStart = std::make_pair(-1, -1);
        m_dragDest = std::make_pair(-1, -1);

        if (m_dragJewelTexture) {
            m_dragJewelTexture = nullptr;
        }
    }
}


void Grid::clearTextureCache() {
    for (const auto& pair : m_textureCache) {
        SDLUtils::destroy(pair.second);
    }
    m_textureCache.clear();
}

void Grid::updateGrid() {
    m_gridTexture = nullptr;
}

void Grid::resetGrid() {
    m_viewModel.reset();
    updateGrid();
}
