#pragma once

#include "engine_export.h"

#include "common/constants.h"
#include "common/runtime_context.h"

#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"

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

    std::shared_ptr<RuntimeContext> m_ctx;
    SDL_Window* m_windowHandle = nullptr;

    std::vector<std::unique_ptr<GameObject>> m_gameObjects;

#if defined(ENABLE_IMGUI)
    int m_selectedGOIdx = 0;
    void onInspectorGUI();
#endif
};
