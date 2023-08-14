#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../external/json.hpp"

#include <unordered_map>
#include <string>

#include "../model/Colour.hpp"

class AssetManager {
public:
	AssetManager(SDL_Renderer* renderer);
	~AssetManager();

	SDL_Texture* getTextureForColour(const Colour::Type& colour);

private:

	SDL_Renderer* m_renderer;
	std::unordered_map<Colour::Type, SDL_Texture*> m_textureMap;

	bool loadColoursFromConfig();
	SDL_Texture* loadTexture(const std::string& imagePath);

};

#endif // ASSET_MANAGER_HPP
