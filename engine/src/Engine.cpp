#include "common/common_include.h"

#include <SDL_syswm.h>

#include <bgfx/platform.h>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <common/imgui/imgui.h>

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
#if defined(_WIN32)
    pd.ndt = nullptr;
    pd.nwh = reinterpret_cast<void*>(wmi.info.win.window);
#else
    LOG_ERROR("Unsupported platform for bgfx initialization");
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
    bgfx::setViewRect(0, 0, 0, uint16_t(m_ctx->window.width), uint16_t(m_ctx->window.height));
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    // Set empty primitive on screen
    bgfx::touch(0);
#pragma endregion

#pragma region Imgui

    // Set view 0 clear state.
    bgfx::setViewClear(255
        , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
        , 0x303030ff
        , 1.0f
        , 0
        );
    imguiCreate();
#pragma endregion

    m_isInitialized = true;
    LOG_INFO("Engine initialized successfully");
}

Engine::~Engine() {
    m_gameObjects.clear();

    imguiDestroy();

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
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                running = false;
            }
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
                m_ctx->window.viewId,
                glm::value_ptr(view),
                glm::value_ptr(proj)
            );
        } else {
            LOG_ERROR("No active camera found!!!");
        }

        bgfx::touch(0); // ensure view 0 is cleared even if nothing submits
#pragma endregion

        // // Update and render all game objects
        // for (const auto& go : m_gameObjects) {
        //     go->update(deltaTime);
        // }

        for (const auto& go : m_gameObjects) {
            go->render();
        }

#pragma region Imgui
        const uint16_t w = (uint16_t)m_ctx->window.width / 2;
        const uint16_t h = (uint16_t)m_ctx->window.height / 2;

        const uint8_t mouseButtons = 0;
        const int32_t mouseX = 0, mouseY = 0, mouseWheel = 0;

        // If your helper supports passing the view id, use it:
        imguiBeginFrame(0
            , 0
            , 0
            , 0
            , w
            , h
            , -1
            , 255
            );

        ImGui::Begin("Debugging");
        ImGui::TextUnformatted("Demo App");
        ImGui::End();

        imguiEndFrame(); // submit drawcall to bgfx

        // Set view 0 default viewport.
        bgfx::setViewRect(255, 0, 0, uint16_t(w), uint16_t(h) );
        bgfx::touch(255);
#pragma endregion

        bgfx::frame();
    }
}

void Engine::addGameObject(std::unique_ptr<GameObject> go) {
    go->setContext(m_ctx);
    m_gameObjects.push_back(std::move(go));
}
