#include "Grid.hpp"
#include "../external/loguru.hpp"

Grid::Grid(GameViewModel& viewModel) : m_viewModel(viewModel), m_highlightedRow(-1), m_highlightedCol(-1) {
}

SDL_Texture* Grid::renderJewel(SDL_Renderer* renderer, const std::string& imagePath) {

    // Load Image
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        LOG_F(ERROR, "Error loading image:  %s", imagePath.c_str());
        return nullptr;
    }

    // Return texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;

}

void Grid::render(SDL_Renderer* renderer, int xOffset, int yOffset) {
    SDL_Texture* gridTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_viewModel.getNumCols() * Constants::JEWEL_SIZE, m_viewModel.getNumRows() * Constants::JEWEL_SIZE);
    if (!gridTexture) {
        LOG_F(ERROR, "Error creating grid texture: %s", SDL_GetError());
        return;
    }

    // LOG_F(INFO, "Creating grid texture...");

    // Set the render target to the grid texture
    SDL_SetRenderTarget(renderer, gridTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Loop through the grid and render each jewel
    for (int row = 0; row < m_viewModel.getNumRows(); ++row) {
        for (int col = 0; col < m_viewModel.getNumCols(); ++col) {
            std::string imagePath = m_viewModel.getColourImgPath(row, col);
            if (!imagePath.empty()) {
                SDL_Texture* jewelTexture = renderJewel(renderer, imagePath);
                if (jewelTexture) {
                    SDL_Rect destRect = {col * Constants::JEWEL_SIZE , row * Constants::JEWEL_SIZE, Constants::JEWEL_SIZE, Constants::JEWEL_SIZE};

                    // Highlight the jewel by drawing a rectangle around it
                    if (m_viewModel.isJewelHighlighted(row, col)) {
                        // LOG_F(INFO, "Highlighting jewel at row %d, col %d", row, col);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                        SDL_RenderDrawRect(renderer, &destRect);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    }

                    SDL_RenderCopy(renderer, jewelTexture, nullptr, &destRect);
                    SDL_DestroyTexture(jewelTexture);
                }
            }
        }
    }

    // LOG_F(INFO, "Finished rendering grid.");

    // Reset the render target
    SDL_SetRenderTarget(renderer, nullptr);

    if (gridTexture) {
        SDL_Rect destRect = {xOffset, yOffset, m_viewModel.getNumCols() * Constants::JEWEL_SIZE, m_viewModel.getNumRows() * Constants::JEWEL_SIZE};
        SDL_RenderCopy(renderer, gridTexture, nullptr, &destRect);
        SDL_DestroyTexture(gridTexture);
    }
}


void Grid::handleMouseClick(int x, int y) {
    // Calculate grid position based on mouse click
    int col = (x) / Constants::JEWEL_SIZE;
    // int col = (x - offset_col) / Constants::JEWEL_SIZE;
    // int row = (y - offset_row) / Constants::JEWEL_SIZE;
    int row = (y) / Constants::JEWEL_SIZE;

    // Check if the click is within the grid bounds
    if (row >= 0 && row < m_viewModel.getNumRows() && col >= 0 && col < m_viewModel.getNumCols()) {
        // Toggle the highlighted state of the clicked jewel
        m_viewModel.toggleJewelHighlight(row, col);
    }
}
