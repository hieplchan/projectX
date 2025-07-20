#pragma once

#include "engine_export.h"

#include "common/constants.h"
#include "common/typedefs.h"

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