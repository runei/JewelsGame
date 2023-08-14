#include "AssetManager.hpp"
#include "Constants.hpp"

#include "../external/loguru.hpp"

#include <fstream>

AssetManager::AssetManager(SDL_Renderer* renderer) : m_renderer(renderer) {

	loadColoursFromConfig();

}

AssetManager::~AssetManager() {
    for (auto& texture : m_textureMap) {
        SDL_DestroyTexture(texture.second);
    }
}

SDL_Texture* AssetManager::getTextureForColour(const Colour::Type& colour) {
    auto textureIt = m_textureMap.find(colour);
    if (textureIt != m_textureMap.end()) {
        return textureIt->second;
    }
    return nullptr;
}

bool AssetManager::loadColoursFromConfig() {

	try {

		// Open config file
		std::ifstream configFile(Constants::COLOUR_CONFIG_PATH);
		if (!configFile.is_open()) {
			LOG_F(ERROR, "Error opening config file: %s", Constants::COLOUR_CONFIG_PATH);
			return false;
		}

		// Create Json object
		nlohmann::json root;
		configFile >> root;

		//Fill texture map
		for (const auto& colourEntry : root["Colours"]) {
			Colour::Type colourType = Colour::GetColorTypeByName(colourEntry["name"].get<std::string>());
			if (colourType != Colour::Type::Unknown) {
				m_textureMap[colourType] = loadTexture(colourEntry["imagePath"].get<std::string>());
			}

		}

		return true;

	} catch (const nlohmann::json::exception& e) {
		LOG_F(ERROR, "Error parsing config file: %s", e.what());
		return false;
	}

}

SDL_Texture* AssetManager::loadTexture(const std::string& imagePath) {

	// Load Image
	SDL_Surface* surface = IMG_Load(imagePath.c_str());
	if (!surface) {
		LOG_F(ERROR, "Error loading image:  %s", imagePath.c_str());
		return nullptr;
	}

	// Return texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);

    return texture;

}

