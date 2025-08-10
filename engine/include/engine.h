#pragma once

#include "engine_export.h"

#include "common/constants.h"
#include "common/typedefs.h"

#include "GameObject.h"

struct SDL_Window;

class ENGINE_EXPORT Engine {
public:
    Engine();
    ~Engine();

    // Contain unique pointers to game objects -> must be non-copyable
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void run();

    void addGameObject(std::unique_ptr<GameObject> go);

private:
    bool m_isInitialized = false;

    WindowSettings m_windowSettings;
    SDL_Window* m_windowHandle = nullptr;

    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};
