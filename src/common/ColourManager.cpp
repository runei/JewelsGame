#include "ColourManager.hpp"
#include "Constants.hpp"

#include "../external/loguru.hpp"

#include <fstream>
#include <random>

ColourManager::ColourManager() {
    // Load colours from configuration when the manager is constructed
    loadColoursFromConfig();
}

ColourManager::~ColourManager() {}

// Find the image path associated with the given colour
std::string ColourManager::getImgPathForColour(const Colour& colour) const {
    auto it = m_colourImgMap.find(colour);
    if (it != m_colourImgMap.end()) {
        return it->second;
    }
    return "";
}

Colour ColourManager::getColourByName(const std::string& name) {
    // Mapping of colour names to Colour enumeration values
    static std::unordered_map<std::string, Colour> nameToColourMap = {
        {"Black", Colour::Black},
        {"White", Colour::White},
        {"Pink", Colour::Pink},
        {"Blue", Colour::Blue},
        {"Orange", Colour::Orange}
    };

    // Find the Colour based on the provided name
    auto it = nameToColourMap.find(name);
    if (it != nameToColourMap.end()) {
        return it->second;
    }
    return Colour::Unknown;
}

Colour ColourManager::getRandomColour() const {
    const int count = static_cast<int>(Colour::Unknown);

    // Generate a random number within the range of available colours
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution<int> colourDistribution(0, count - 1);

    // Convert the random number to a Colour enumeration value
    return static_cast<Colour>(colourDistribution(gen));
}

bool ColourManager::loadColoursFromConfig() {
    LOG_SCOPE_FUNCTION(INFO);

    LOG_F(INFO, "Loading colour configuration from: %s", Constants::COLOUR_CONFIG_PATH);

    std::ifstream configFile(Constants::COLOUR_CONFIG_PATH);
    if (!configFile.is_open()) {
        LOG_F(ERROR, "Error opening config file: %s", Constants::COLOUR_CONFIG_PATH);
        return false;
    }

    try {
        nlohmann::json root;
        configFile >> root;

        LOG_F(INFO, "Parsing colour configuration...");

        const nlohmann::json& coloursObject = root["Colours"];
        for (auto& entry : coloursObject.items()) {
            const std::string& colourName = entry.key();
            const std::string& imagePath = entry.value().get<std::string>();

            // Convert the colour name to a Colour enumeration value
            Colour colourType = getColourByName(colourName);
            if (colourType != Colour::Unknown) {
                // Store the image path associated with the colour
                m_colourImgMap.emplace(colourType, imagePath);
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
