#ifndef SDL_EXCEPTION_HPP
#define SDL_EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include "SDL2/SDL.h"

// SDLException class extends std::runtime_error to handle SDL-related exceptions
class SDLException : public std::runtime_error {
public:
    // Constructor with message describing the exception
    SDLException(const std::string& msg) : std::runtime_error(msg) {}

    // Constructor with both a message and an SDL error message
    SDLException(const std::string& msg, const std::string& sdlError) : std::runtime_error(msg), m_sdlError(sdlError) {}

    // Getter for the SDL error message
    const char* getSdlError() const {
        return m_sdlError.c_str();
    }

private:
    std::string m_sdlError; // Holds the SDL-specific error message
};

#endif // SDL_EXCEPTION_HPP
