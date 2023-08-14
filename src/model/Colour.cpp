#include "Colour.hpp"

Colour::Type Colour::GetColorTypeByName(const std::string& name) {

	static std::unordered_map<std::string, Type> nameToTypeMap = {
		{"Black", Type::Black},
        {"White", Type::White},
        {"Pink", Type::Pink},
        {"Blue", Type::Blue},
        {"Orange", Type::Orange}
	};

	auto it = nameToTypeMap.find(name);
	if (it != nameToTypeMap.end()) {
		return it->second;
	}

	return Type::Unknown;

}
