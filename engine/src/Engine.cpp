#include "common/common_include.h"

#include <SDL_syswm.h>

#include <bgfx/platform.h>
#include <glm/gtc/type_ptr.hpp>

#if defined(ENABLE_IMGUI)
#include <imgui_impl_bgfx.h>
#include <imgui_impl_sdl2.h>
#endif

#include "Engine.h"

Engine::Engine()
    : m_ctx(std::make_shared<RuntimeContext>()) {
    LOG_INFO("constructing...");

#pragma region SDL
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        LOG_ERROR(std::format("SDL_Init failed: {}", SDL_GetError()));
        return;
    }

    // Create a window
    m_windowHandle = SDL_CreateWindow(m_ctx->window.title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_ctx->window.width, m_ctx->window.height,
        SDL_WINDOW_SHOWN);

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
#pragma endregion

#pragma region BGFX
    bgfx::PlatformData pd;
    bgfx_renderer_type_t renderer_type = BGFX_RENDERER_TYPE_COUNT;
#if BX_PLATFORM_WINDOWS
    pd.nwh = wmi.info.win.window;
    renderer_type = BGFX_RENDERER_TYPE_DIRECT3D11;
#elif BX_PLATFORM_OSX
    pd.nwh = wmi.info.cocoa.window;
    renderer_type = BGFX_RENDERER_TYPE_METAL;
#elif BX_PLATFORM_LINUX
    pd.ndt = wmi.info.x11.display;
    pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
    renderer_type = BGFX_RENDERER_TYPE_OPENGL;
#endif

    bgfx::setPlatformData(pd);

    // Initialize bgfx
    bgfx::Init init;
    init.type = bgfx::RendererType::Count; // Let bgfx choose the best renderer
    init.resolution.width = m_ctx->window.width;
    init.resolution.height = m_ctx->window.height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData = pd;

    if (!bgfx::init(init)) {
        LOG_ERROR("bgfx::init failed");
        return;
    }

    bgfx::reset(m_ctx->window.width, m_ctx->window.height, BGFX_RESET_VSYNC);
    bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);

    // World View Layer
    bgfx::setViewName(m_ctx->window.viewIds.world, "world");
    bgfx::setViewRect(
        m_ctx->window.viewIds.world,
        0, 0,
        m_ctx->window.width, m_ctx->window.height
    );
    bgfx::setViewClear(
        m_ctx->window.viewIds.world,
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
        0x443355FF, 1.0f, 0
    );
    bgfx::touch(m_ctx->window.viewIds.world);
#pragma endregion

#if defined(ENABLE_IMGUI)
    ImGui::CreateContext();
    ImGui_Implbgfx_Init(static_cast<int>(m_ctx->window.viewIds.ui), m_ctx->window.msaaSamples, m_ctx->window.bUsingVSync);
#if BX_PLATFORM_WINDOWS
    ImGui_ImplSDL2_InitForD3D(m_windowHandle);
#elif BX_PLATFORM_OSX
    ImGui_ImplSDL2_InitForMetal(m_windowHandle);
#elif BX_PLATFORM_LINUX
    ImGui_ImplSDL2_InitForOpenGL(m_windowHandle, nullptr);
#endif
#endif
    m_isInitialized = true;
    LOG_INFO("Engine initialized successfully");
}

Engine::~Engine() {
    m_gameObjects.clear();

#if defined(ENABLE_IMGUI)
    ImGui_ImplSDL2_Shutdown();
    ImGui_Implbgfx_Shutdown();
    ImGui::DestroyContext();
#endif

    bgfx::shutdown();

    if (m_windowHandle) {
        SDL_DestroyWindow(m_windowHandle);
        m_windowHandle = nullptr;
    }
    SDL_Quit();

    LOG_INFO("destructed");
}

void Engine::run() {
    LOG_INFO("Game loop started");

    auto lastFrameTime = std::chrono::steady_clock::now();

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

#if defined(ENABLE_IMGUI)
            ImGui_ImplSDL2_ProcessEvent(&event);
#endif
        }

        auto now = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(now - lastFrameTime).count();
        lastFrameTime = now;

#pragma region Camera
        const Camera* activeCam = nullptr;
        const GameObject* camOwnerGO = nullptr;

        for (auto& go: m_gameObjects) {
            if (auto* cam = go->getComponent<Camera>()) {
                activeCam = cam;
                camOwnerGO = go.get();
                break;
            }
        }

        // first camera found is main camera
        if (activeCam && camOwnerGO) {
            const glm::mat4 view = activeCam->view();
            const glm::mat4 proj = activeCam->proj();

            bgfx::setViewTransform(
                m_ctx->window.viewIds.world,
                glm::value_ptr(view),
                glm::value_ptr(proj)
            );
        } else {
            LOG_ERROR("No active camera found!!!");
        }

        bgfx::touch(m_ctx->window.viewIds.world);
#pragma endregion

        // // Update and render all game objects
        // for (const auto& go : m_gameObjects) {
        //     go->update(deltaTime);
        // }

        for (const auto& go : m_gameObjects) {
            go->render();
        }

#if defined(ENABLE_IMGUI)
        ImGui_Implbgfx_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Debugging");

        ImGui::TextUnformatted("Demo App");
        if (ImGui::Button("Click Me")) {
            LOG_INFO("Button was clicked!");
        }

        ImGui::End();

        ImGui::Render();
        ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
#endif
        bgfx::touch(m_ctx->window.viewIds.ui);
#pragma endregion

        bgfx::frame();
    }
}

void Engine::addGameObject(std::unique_ptr<GameObject> go) {
    go->setContext(m_ctx);
    m_gameObjects.push_back(std::move(go));
}
