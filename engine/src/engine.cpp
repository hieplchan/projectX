#include <iostream>

#include <SDL.h>
#include <bgfx/bgfx.h>

#include "engine.h"

Engine::Engine() {
    std::cout << "Engine::Engine()" << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return;
    }

    windowHandle = SDL_CreateWindow("Engine Window", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        800, 600, SDL_WINDOW_SHOWN || SDL_WINDOW_ALLOW_HIGHDPI);

    if (!windowHandle) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return;
    }
}

Engine::~Engine() {
    std::cout << "Engine::~Engine()" << std::endl;

    SDL_DestroyWindow(windowHandle);
    SDL_Quit();
    windowHandle = nullptr;
}