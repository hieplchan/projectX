#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

static const uint32_t WIDTH = 800;
static const uint32_t HEIGHT = 600;

int main(int argc, char **argv)
{
    // 1) SDL init + window
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "BGFX+SDL2 on Windows",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // 2) Get native HWND
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    SDL_GetWindowWMInfo(window, &wmi);

    // 3) Supply platform data to BGFX
    bgfx::PlatformData pd;
    pd.ndt = nullptr; // no X11 Display
    pd.nwh = reinterpret_cast<void *>(wmi.info.win.window);
    bgfx::setPlatformData(pd);

    // 4) Proper BGFX init
    bgfx::Init init;
    init.type = bgfx::RendererType::Count; // auto-select
    init.resolution.width = WIDTH;
    init.resolution.height = HEIGHT;
    init.resolution.reset = BGFX_RESET_NONE;
    // ← ADD THIS LINE:
    init.platformData      = pd;
    
    if (!bgfx::init(init))
    {
        SDL_Log("BGFX initialization failed");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // 5) Main loop
    bool quit = false;
    SDL_Event ev;
    while (!quit)
    {
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                quit = true;
            else if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                // tell BGFX about the new size
                uint32_t w = ev.window.data1;
                uint32_t h = ev.window.data2;
                bgfx::reset(w, h, BGFX_RESET_NONE);
                bgfx::setViewRect(0, 0, 0, w, h);
            }
        }

        // tell BGFX “we want to render view 0” (even if we draw nothing)
        bgfx::touch(0);

        // submit frame — this kicks off rendering
        bgfx::frame();
    }

    // 6) Cleanup
    bgfx::shutdown();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
