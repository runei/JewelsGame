#include "Grid.hpp"
#include "../external/loguru.hpp"

Grid::Grid(GameViewModel& viewModel)
    : m_viewModel(viewModel), m_gridTexture(nullptr), m_dragging(false), m_dragJewelTexture(nullptr) {
}

Grid::~Grid() {
    if (m_gridTexture) {
        SDL_DestroyTexture(m_gridTexture);
    }
    clearTextureCache();
}

SDL_Texture* Grid::getOrCreateTexture(SDL_Renderer* renderer, const std::string& imagePath) {
    if (m_textureCache.count(imagePath)) {
        return m_textureCache[imagePath];
    }

    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        LOG_F(ERROR, "Error loading image: %s", imagePath.c_str());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (texture) {
        m_textureCache[imagePath] = texture;
    } else {
        LOG_F(ERROR, "Error creating texture for image: %s", SDL_GetError());
    }

    return texture;
}

void Grid::render(SDL_Renderer* renderer) {
    if (!m_gridTexture) {
        createGridTexture(renderer);
    }

    // Set the rendering viewport to match the grid size
    SDL_Rect viewport = {0, 0, m_viewModel.getNumCols() * Constants::JEWEL_SIZE, m_viewModel.getNumRows() * Constants::JEWEL_SIZE};
    SDL_RenderSetViewport(renderer, &viewport);

    // Render the grid texture
    SDL_RenderCopy(renderer, m_gridTexture, nullptr, nullptr);

    // Reset the rendering viewport
    SDL_RenderSetViewport(renderer, nullptr);

    if (m_dragging) {
        if (m_dragJewelTexture) {
            int destX = m_dragDestCol - Constants::JEWEL_SIZE / 2;
            int destY = m_dragDestRow - Constants::JEWEL_SIZE / 2;

            SDL_Rect destRect = {destX, destY, Constants::JEWEL_SIZE, Constants::JEWEL_SIZE};
            SDL_RenderCopy(renderer, m_dragJewelTexture, nullptr, &destRect);
        }
    }
}

void Grid::createGridTexture(SDL_Renderer* renderer) {
    m_gridTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_viewModel.getNumCols() * Constants::JEWEL_SIZE, m_viewModel.getNumRows() * Constants::JEWEL_SIZE);


    if (!m_gridTexture) {
        LOG_F(ERROR, "Error creating grid texture: %s", SDL_GetError());
        return;
    }

    SDL_SetRenderTarget(renderer, m_gridTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    for (int row = 0; row < m_viewModel.getNumRows(); ++row) {
        for (int col = 0; col < m_viewModel.getNumCols(); ++col) {
            std::string imagePath = m_viewModel.getColourImgPath(row, col);
            if (!imagePath.empty()) {
                SDL_Texture* jewelTexture = getOrCreateTexture(renderer, imagePath);
                if (jewelTexture) {
                    int destX = col * Constants::JEWEL_SIZE;
                    int destY = row * Constants::JEWEL_SIZE;

                    if (m_viewModel.isJewelHighlighted(row, col)) {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                        SDL_Rect highlightRect = {destX, destY, Constants::JEWEL_SIZE, Constants::JEWEL_SIZE};
                        SDL_RenderDrawRect(renderer, &highlightRect);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    }

                    SDL_Rect destRect = {destX, destY, Constants::JEWEL_SIZE, Constants::JEWEL_SIZE};
                    SDL_RenderCopy(renderer, jewelTexture, nullptr, &destRect);
                }
            }
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);
}

void Grid::handleMouseClick(int x, int y, SDL_Renderer* renderer) {
    int col = x / Constants::JEWEL_SIZE;
    int row = y / Constants::JEWEL_SIZE;

    if (col >= 0 && col < m_viewModel.getNumCols() && row >= 0 && row < m_viewModel.getNumRows()) {
        if (m_viewModel.toggleJewelHighlight(row, col)) {

            m_dragging = true;
            m_dragStartCol = col;
            m_dragStartRow = row;
            m_dragDestCol = col * Constants::JEWEL_SIZE + Constants::JEWEL_SIZE / 2;
            m_dragDestRow = row * Constants::JEWEL_SIZE + Constants::JEWEL_SIZE / 2;

            // Load the jewel texture for dragging
            std::string imagePath = m_viewModel.getColourImgPath(m_dragStartRow, m_dragStartCol);
            m_dragJewelTexture = getOrCreateTexture(renderer, imagePath);

            m_gridTexture = nullptr;

            LOG_F(INFO, "Grid texture reset.");
        }
    }
}

void Grid::handleMouseMotion(int x, int y) {
    if (m_dragging) {
        // Update destination position during drag movement
        m_dragDestCol = x;
        m_dragDestRow = y;
    }
}

void Grid::handleMouseRelease(int x, int y) {
    if (m_dragging) {

        int col = x / Constants::JEWEL_SIZE;
        int row = y / Constants::JEWEL_SIZE;

        if ((row == m_dragStartRow && abs(col - m_dragStartCol) == 1) || (col == m_dragStartCol && abs(row - m_dragStartRow) == 1)) {

            m_viewModel.swapJewels(row, col, m_dragStartRow, m_dragStartCol);

            m_gridTexture = nullptr;

            LOG_F(INFO, "Grid texture release reset.");
        }

        m_dragging = false;
        m_dragStartCol = -1;
        m_dragStartRow = -1;
        m_dragDestCol = -1;
        m_dragDestRow = -1;

        if (m_dragJewelTexture) {
            m_dragJewelTexture = nullptr;
        }
    }
}

void Grid::clearTextureCache() {
    for (const auto& pair : m_textureCache) {
        SDL_DestroyTexture(pair.second);
    }
    m_textureCache.clear();
}
