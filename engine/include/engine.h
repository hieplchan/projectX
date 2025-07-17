#pragma once

#include "engine_export.h"
#include "constants.h"

struct SDL_Window;

class ENGINE_EXPORT Engine {
public:
    Engine();
    ~Engine();

    void run();

private:
	bool m_isInitialized = false;

    WindowSettings m_windowSettings;
    SDL_Window* m_windowHandle = nullptr;
};