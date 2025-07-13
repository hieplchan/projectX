#include <iostream>

#include <SDL.h>
#include <bgfx/bgfx.h>

#include "engine.h"

Engine::Engine() {
    std::cout << "Engine::Engine()" << std::endl;

    SDL_version sdlVersion;
    SDL_VERSION(&sdlVersion);
    std::cout << "SDL2 version: "
                << static_cast<int>(sdlVersion.major) << "."
                << static_cast<int>(sdlVersion.minor) << "."
                << static_cast<int>(sdlVersion.patch) << std::endl;

    std::cout << "bgfx Vendor ID (still not init yet): " 
                << static_cast<int>(bgfx::getCaps()->vendorId) 
                << std::endl;
}

Engine::~Engine() {
    std::cout << "Engine::~Engine()" << std::endl;
}