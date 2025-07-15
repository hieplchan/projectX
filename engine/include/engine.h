#pragma once

#include "engine_export.h"

struct SDL_Window;

class ENGINE_EXPORT Engine {
public:
    Engine();
    ~Engine();

private:
    SDL_Window* windowHandle = nullptr;
};