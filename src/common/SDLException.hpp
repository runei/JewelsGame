#ifndef SDL_EXCEPTION_HPP
#define SDL_EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include "SDL2/SDL.h"

class SDLException : public std::runtime_error {
public:

    SDLException(const std::string& msg) : std::runtime_error(msg) {}
    SDLException(const std::string& msg, const std::string& sdlError) : std::runtime_error(msg), m_sdlError(sdlError) {}


    const char* getSdlError() const {
        return m_sdlError.c_str();
    }


private:
    std::string m_sdlError;
};

#endif // SDL_EXCEPTION_HPP
