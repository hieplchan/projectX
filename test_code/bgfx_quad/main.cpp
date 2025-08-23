// https://www.sandeepnambiar.com/getting-started-with-bgfx/

#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>
#include <vector>

static const uint32_t WIDTH = 800;
static const uint32_t HEIGHT = 600;

bgfx::ShaderHandle loadShader(const char* _name) {
    std::ifstream file(_name, std::ios::binary);
    if (!file)
    {
        SDL_Log("Failed to open shader: %s", _name);
        return BGFX_INVALID_HANDLE;
    }
    file.seekg(0, std::ios::end);
    auto fileSize = size_t(file.tellg());
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> data(fileSize);
    file.read(reinterpret_cast<char*>(data.data()), fileSize);
    const bgfx::Memory* mem = bgfx::copy(data.data(), fileSize);

    SDL_Log("Loading shader: %s, size: %zu bytes", _name, fileSize);
    return bgfx::createShader(mem);
}

struct PosColorVertex
{
    float m_x;
    float m_y;
    float m_z;
    uint32_t m_abgr;

    static void init() {
        ms_decl
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    };

    static bgfx::VertexLayout ms_decl;
};
bgfx::VertexLayout PosColorVertex::ms_decl;

static PosColorVertex s_cubeVertices[] =
{
    {0.5f, 0.5f, 0.0f, 0xff0000ff},
    {0.5f, -0.5f, 0.0f, 0xff0000ff},
    {-0.5f, -0.5f, 0.0f, 0xff00ff00},
    {-0.5f, 0.5f, 0.0f, 0xff00ff00}
};

static const uint16_t s_cubeTriList[] =
{
    0, 1, 3,
    1, 2, 3
};

bgfx::VertexBufferHandle m_vbh;
bgfx::IndexBufferHandle m_ibh;
bgfx::ProgramHandle m_program;

int main(int argc, char** argv) {
    // SDL initialization
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "BGFX test quad",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window)
    {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // BGFX initialization
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    SDL_GetWindowWMInfo(window, &wmi);

    bgfx::PlatformData pd;
    pd.ndt = nullptr;
    pd.nwh = reinterpret_cast<void*>(wmi.info.win.window);
    bgfx::setPlatformData(pd);

    bgfx::Init init;
    init.type = bgfx::RendererType::Count; // auto-select
    init.resolution.width = WIDTH;
    init.resolution.height = HEIGHT;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData = pd;

    if (!bgfx::init(init))
    {
        SDL_Log("BGFX initialization failed");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

#pragma region Load Shaders
    PosColorVertex::init();
    m_vbh = bgfx::createVertexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
        PosColorVertex::ms_decl);

    m_ibh = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
    bgfx::ShaderHandle vsh = loadShader("vs_quad.bin");
    bgfx::ShaderHandle fsh = loadShader("fs_quad.bin");
    m_program = bgfx::createProgram(vsh, fsh, true);
#pragma endregion

    bgfx::reset(WIDTH, HEIGHT, BGFX_RESET_VSYNC);
    bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);
    bgfx::setViewRect(0, 0, 0, uint16_t(WIDTH), uint16_t(HEIGHT));
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);

    // Set empty primitive on screen
    bgfx::touch(0);

    // Main loop
    bool quit = false;
    SDL_Event currentEvent;
    while (!quit)
    {
        while (SDL_PollEvent(&currentEvent) != 0)
        {
            if (currentEvent.type == SDL_QUIT)
            {
                quit = true;
            }
        }

#pragma region Camera
        const bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
        const bx::Vec3 eye = { 0.0f, 0.0f, 10.0f };

        // Set view and projection matrix for view 0.
        float view[16];
        bx::mtxLookAt(view, eye, at);

        float proj[16];
        bx::mtxProj(proj,
            60.0f,
            float(WIDTH) / float(HEIGHT),
            0.1f, 100.0f,
            bgfx::getCaps()->homogeneousDepth);

        bgfx::setViewTransform(0, view, proj);

        // Set view 0 default viewport.
        bgfx::setViewRect(0, 0, 0,
            WIDTH,
            HEIGHT);

        bgfx::touch(0);
#pragma endregion

#pragma region Object
        float mtx[16];
        bx::mtxRotateY(mtx, 0.0f);

        // position x,y,z
        mtx[12] = 0.0f;
        mtx[13] = 0.0f;
        mtx[14] = 0.0f;

        // Set model matrix for rendering.
        bgfx::setTransform(mtx);

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);

        // Set render states.
        bgfx::setState(BGFX_STATE_DEFAULT);

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, m_program);
#pragma endregion

        bgfx::frame();
    }

    // Cleanup
    bgfx::shutdown();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
