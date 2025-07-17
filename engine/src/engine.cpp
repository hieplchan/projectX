#include <iostream>

#include <SDL.h>
#include <SDL_syswm.h>
#include <bgfx/platform.h>
#include <bgfx/bgfx.h>

#include "engine.h"

Engine::Engine() {
    std::cout << "Engine::Engine()" << std::endl;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return;
    }

    // Create a window
    m_windowHandle = SDL_CreateWindow(m_windowSettings.title.c_str(), 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        m_windowSettings.width, m_windowSettings.height, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    if (!m_windowHandle) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return;
    }

    //Retrieve native window handle for bgfx
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(m_windowHandle, &wmi)) {
        std::cerr << "SDL_GetWindowWMInfo failed: " << SDL_GetError() << std::endl;
        return;
    }

    bgfx::PlatformData pd;
#if defined(_WIN32)
    pd.ndt = nullptr;
    pd.nwh = wmi.info.win.window;
#else
	std::cout << "Unsupported platform for bgfx initialization" << std::endl;
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
        std::cerr << "bgfx::init failed" << std::endl;
        return;
    }

    // Setup default view 0
    bgfx::setViewClear(m_windowSettings.viewId, 
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 
        0x303030ff, // dark gray background
        1.0f, 0);
    bgfx::setViewRect(m_windowSettings.viewId, 0, 0, init.resolution.width, init.resolution.height);

    m_isInitialized = true;
    std::cout << "Engine initialized successfully" << std::endl;
}

Engine::~Engine() {
    std::cout << "Engine::~Engine()" << std::endl;

    if (m_windowHandle) {
        SDL_DestroyWindow(m_windowHandle);
        m_windowHandle = nullptr;
    }
    SDL_Quit();
}

void Engine::run() {
    std::cout << "Engine::run()" << std::endl;

    bool running = true;
    while (running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                running = false;
            }
        }
        // Ensure view 0 is cleared and rendered
        bgfx::touch(0);
        bgfx::frame();
    }
}