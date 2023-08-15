#include "ColourManager.hpp"
#include "Constants.hpp"

#include "../external/loguru.hpp"

#include <fstream>

ColourManager::ColourManager() {

	loadColoursFromConfig();

}

ColourManager::~ColourManager() {
}

std::string ColourManager::getImgPathForColour(const Colour& colour) const {
    auto it = m_colourImgMap.find(colour);
    if (it != m_colourImgMap.end()) {
        return it->second;
    }
    return "";
}

Colour ColourManager::getColourByName(const std::string& name) {

	static std::unordered_map<std::string, Colour> nameToColourMap = {
		{"Black", Colour::Black},
        {"White", Colour::White},
        {"Pink", Colour::Pink},
        {"Blue", Colour::Blue},
        {"Orange", Colour::Orange}
	};

	auto it = nameToColourMap.find(name);
	if (it != nameToColourMap.end()) {
		return it->second;
	}

	return Colour::Unknown;

}


bool ColourManager::loadColoursFromConfig() {
    try {
        LOG_F(INFO, "Loading colour configuration from: %s", Constants::COLOUR_CONFIG_PATH);

        // Open config file
        std::ifstream configFile(Constants::COLOUR_CONFIG_PATH);
        if (!configFile.is_open()) {
            LOG_F(ERROR, "Error opening config file: %s", Constants::COLOUR_CONFIG_PATH);
            return false;
        }

        // Create Json object
        nlohmann::json root;
        configFile >> root;

        LOG_F(INFO, "Parsing colour configuration...");

        // Fill texture map
        const nlohmann::json& coloursObject = root["Colours"];
		for (auto it = coloursObject.begin(); it != coloursObject.end(); ++it) {
		    const std::string& colourName = it.key(); // The key is the colour name
		    const std::string& imagePath = it.value().get<std::string>(); // The value is the image path

		    Colour colourType = getColourByName(colourName);
		    if (colourType != Colour::Unknown) {
		        m_colourImgMap[colourType] = imagePath;
		        LOG_F(INFO, "Loaded colour: %s - Image Path: %s", colourName.c_str(), imagePath.c_str());
		    } else {
		        LOG_F(WARNING, "Unknown colour: %s, ignoring...", colourName.c_str());
		    }
		}

        LOG_F(INFO, "Colour configuration loaded successfully.");
        return true;
    } catch (const nlohmann::json::exception& e) {
        LOG_F(ERROR, "Error parsing config file: %s", e.what());
        return false;
    }
}

