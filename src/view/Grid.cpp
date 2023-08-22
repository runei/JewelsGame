#include "Grid.hpp"
#include "../external/loguru.hpp"
#include "../common/SDLUtils.hpp"

Grid::Grid(SDL_Renderer** renderer, GameViewModel& viewModel)
    : m_renderer(renderer), m_viewModel(viewModel), m_gridTexture(nullptr), m_dragging(false), m_dragJewelTexture(nullptr) {
}

Grid::~Grid() {
    // Destroy grid and drag jewel textures
    SDLUtils::destroy(m_gridTexture);
    SDLUtils::destroy(m_dragJewelTexture);

    // Clear the texture cache
    clearTextureCache();
}

SDL_Texture* Grid::getOrCreateTexture(const std::string& imagePath) {
    // Check if the texture is already in the cache
    if (m_textureCache.count(imagePath)) {
        return m_textureCache[imagePath];
    }

    SDL_Texture* texture = nullptr;

    try {
        // Load the image and store it in the texture cache
        texture = SDLUtils::loadImage(*m_renderer, imagePath);
        m_textureCache[imagePath] = texture;
    } catch (const SDLException& e) {
        LOG_F(ERROR, "%s: %s", e.what(), e.getSdlError());
    }

    return texture;
}

void Grid::render() {
    // Ensure the grid texture is created before rendering
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

    // Render the dragging jewel texture, if applicable
    renderDraggingJewel();
}

void Grid::renderDraggingJewel() {
    if (m_dragging && m_dragJewelTexture) {
        int destX = m_dragDest.first - Constants::JEWEL_SIZE / 2;
        int destY = m_dragDest.second - Constants::JEWEL_SIZE / 2;

        // Calculate the destination rectangle for rendering
        SDL_Rect destRect = {destX, destY, Constants::JEWEL_SIZE, Constants::JEWEL_SIZE};

        // Render the dragging jewel texture
        SDL_RenderCopy(*m_renderer, m_dragJewelTexture, nullptr, &destRect);
    }
}


void Grid::createGridTexture() {
    // Load the background image
    SDL_Texture* backgroundTexture = SDLUtils::loadImage(*m_renderer, "assets/images/background.jpg");
    if (!backgroundTexture) {
        LOG_F(ERROR, "Error loading background image: %s", SDL_GetError());
        return;
    }

    // Calculate the dimensions of the grid texture
    int imageWidth = m_viewModel.getNumCols() * Constants::JEWEL_SIZE;
    int imageHeight = m_viewModel.getNumRows() * Constants::JEWEL_SIZE;

    // Create the grid texture
    m_gridTexture = SDL_CreateTexture(*m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, imageWidth, imageHeight);
    if (!m_gridTexture) {
        LOG_F(ERROR, "Error creating grid texture: %s", SDL_GetError());
        SDL_DestroyTexture(backgroundTexture);
        return;
    }

    // Set the render target to the grid texture
    SDL_SetRenderTarget(*m_renderer, m_gridTexture);

    // Clear the grid texture with a transparent color (0 alpha)
    SDL_SetRenderDrawColor(*m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(*m_renderer);

    // Render the background image onto the grid texture
    SDL_RenderCopy(*m_renderer, backgroundTexture, nullptr, nullptr);

    // Render jewels and highlights on top of the background
    renderJewelsWithHighlights();

    // Reset render target and clean up resources
    SDL_SetRenderTarget(*m_renderer, nullptr);
    SDL_DestroyTexture(backgroundTexture);
}

void Grid::renderJewelsWithHighlights() {
    for (int row = 0; row < m_viewModel.getNumRows(); ++row) {
        for (int col = 0; col < m_viewModel.getNumCols(); ++col) {
            renderJewelWithHighlight(row, col);
        }
    }
}

void Grid::renderJewelWithHighlight(int row, int col) {
    // Get the jewel texture for the current position
    std::string imagePath = m_viewModel.getColourImgPath(row, col);
    if (!imagePath.empty()) {
        SDL_Texture* jewelTexture = getOrCreateTexture(imagePath);
        if (jewelTexture) {
            int destX = col * Constants::JEWEL_SIZE;
            int destY = row * Constants::JEWEL_SIZE;

            // Render the highlight if the jewel is highlighted
            if (m_viewModel.isJewelHighlighted(row, col)) {
                renderHighlight(destX, destY);
            }

            // Render the jewel texture
            renderJewel(destX, destY, jewelTexture);
        }
    }
}

void Grid::renderHighlight(int x, int y) {
    SDL_SetRenderDrawColor(*m_renderer, 255, 255, 0, 255);
    SDL_Rect highlightRect = {x, y, Constants::JEWEL_SIZE, Constants::JEWEL_SIZE};
    SDL_RenderDrawRect(*m_renderer, &highlightRect);
    SDL_SetRenderDrawColor(*m_renderer, 0, 0, 0, 255);
}

void Grid::renderJewel(int x, int y, SDL_Texture* jewelTexture) {
    SDL_Rect destRect = {x, y, Constants::JEWEL_SIZE, Constants::JEWEL_SIZE};
    SDL_RenderCopy(*m_renderer, jewelTexture, nullptr, &destRect);
}

// Calculate the clicked row and column based on the mouse coordinates
void Grid::handleMouseClick(int x, int y) {
    int row = y / Constants::JEWEL_SIZE;
    int col = x / Constants::JEWEL_SIZE;

    // Check if the clicked position is within the grid bounds
    if (isValidGridPosition(row, col)) {
        // Toggle the highlight of the clicked jewel and initiate dragging
        if (m_viewModel.toggleJewelHighlight(row, col)) {

            // Initialize dragging state
            initializeDragging(row, col);

            // Load and set up the texture for the dragged jewel
            setupDragJewelTexture(row, col);

            // Update the grid and render
            updateGrid();

            LOG_F(INFO, "Grid texture reset.");
        }
    }
}

bool Grid::isValidGridPosition(int row, int col) const {
    return row >= 0 && row < m_viewModel.getNumRows() && col >= 0 && col < m_viewModel.getNumCols();
}

void Grid::initializeDragging(int row, int col) {
    m_dragging = true;
    m_dragStart = std::make_pair(row, col);
    m_dragDest.first = col * Constants::JEWEL_SIZE + Constants::JEWEL_SIZE / 2;
    m_dragDest.second = row * Constants::JEWEL_SIZE + Constants::JEWEL_SIZE / 2;
}

void Grid::setupDragJewelTexture(int row, int col) {
    // Load the jewel texture for dragging
    std::string imagePath = m_viewModel.getColourImgPath(row, col);
    m_dragJewelTexture = getOrCreateTexture(imagePath);
}

// Update destination position during drag movement
void Grid::handleMouseMotion(int x, int y) {
    if (m_dragging) {
        m_dragDest = std::make_pair(x, y);
    }
}

void Grid::handleMouseRelease(int x, int y) {
    if (m_dragging) {
        const int col = x / Constants::JEWEL_SIZE;
        const int row = y / Constants::JEWEL_SIZE;

        // Check if the released position is adjacent to the start of the drag
        if (isAdjacentToStart(row, col)) {
            // Swap jewels and reset grid texture
            m_viewModel.swapJewels(std::make_pair(row, col), m_dragStart);
            m_gridTexture = nullptr;
            LOG_F(INFO, "Grid texture release reset.");
        }

        // Reset drag-related variables
        resetDragState();
    }
}

// Check if the released position is adjacent to the start of the drag
bool Grid::isAdjacentToStart(int row, int col) const {
    return (row == m_dragStart.first && abs(col - m_dragStart.second) == 1) ||
           (col == m_dragStart.second && abs(row - m_dragStart.first) == 1);
}

void Grid::resetDragState() {
    m_dragging = false;
    m_dragStart = std::make_pair(-1, -1);
    m_dragDest = std::make_pair(-1, -1);

    // Clean up drag jewel texture
    if (m_dragJewelTexture) {
        m_dragJewelTexture = nullptr;
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
