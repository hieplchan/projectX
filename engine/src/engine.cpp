#include "common/common_include.h"

#include <SDL_syswm.h>
#include <bgfx/platform.h>
#include <bx/math.h>

#include "Engine.h"

Engine::Engine() {
    LOG_INFO("Engine::Engine()");

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        LOG_ERROR(std::format("SDL_Init failed: {}", SDL_GetError()));
        return;
    }

    // Create a window
    m_windowHandle = SDL_CreateWindow(m_windowSettings.title.c_str(), 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        m_windowSettings.width, m_windowSettings.height, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    if (!m_windowHandle) {
        LOG_ERROR(std::format("SDL_CreateWindow failed: {}", SDL_GetError()));
        return;
    }

    //Retrieve native window handle for bgfx
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(m_windowHandle, &wmi)) {
        LOG_ERROR(std::format("SDL_GetWindowWMInfo failed: {}", SDL_GetError()));
        return;
    }

    bgfx::PlatformData pd;
#if defined(_WIN32)
    pd.ndt = nullptr;
    pd.nwh = wmi.info.win.window;
#else
	LOG("Unsupported platform for bgfx initialization");
#endif
    bgfx::setPlatformData(pd);

    // Initialize bgfx
    bgfx::Init init;
    init.type = bgfx::RendererType::Count; // Let bgfx choose the best renderer
    init.resolution.width = m_windowSettings.width;
    init.resolution.height = m_windowSettings.height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData = pd;

    if (!bgfx::init(init)) {
        LOG_ERROR("bgfx::init failed");
        return;
    }

    // Setup default view 0
    bgfx::setViewClear(m_windowSettings.viewId, 
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 
        0x303030ff, // dark gray background
        1.0f, 0);
    bgfx::setViewRect(m_windowSettings.viewId, 0, 0, init.resolution.width, init.resolution.height);

    m_isInitialized = true;
    LOG_INFO("Engine initialized successfully");
}

Engine::~Engine() {
    LOG_INFO("Engine::~Engine()");

    if (m_windowHandle) {
        SDL_DestroyWindow(m_windowHandle);
        m_windowHandle = nullptr;
    }
    SDL_Quit();
}

void Engine::run() {
    LOG_INFO("Engine::run()");

    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    
    bool running = true;
    while (running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                running = false;
            }
        }

        auto now = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(now - lastFrameTime).count();
        lastFrameTime = now;

        // Set up view matrix (ortho)
        float proj[16];
        bx::mtxOrtho(
            proj,
            0.0f,
            float(m_windowSettings.width),
            float(m_windowSettings.height),
            0.0f,
            0.0f, 1000.0f,
            0.0f, // homogeneousDepth
            bgfx::getCaps()->homogeneousDepth
        );

        float view[16];
        bx::mtxIdentity(view);

        bgfx::setViewTransform(0, view, proj);
        bgfx::setViewRect(0, 0, 0, uint16_t(m_windowSettings.width), uint16_t(m_windowSettings.height));
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

        // Update and render all game objects
        for (const auto& go : m_gameObjects) {
            go->update(deltaTime);
        }

        for (const auto& go : m_gameObjects) {
            go->render();
        }

        // Ensure view 0 is cleared and rendered
        // bgfx::touch(0);
        bgfx::frame();
    }
}

void Engine::addGameObject(std::unique_ptr<GameObject> go)
{
    m_gameObjects.push_back(std::move(go));
}
