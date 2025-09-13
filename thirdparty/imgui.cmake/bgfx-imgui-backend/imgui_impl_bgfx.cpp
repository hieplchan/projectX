// Derived from this Gist by Richard Gale:
//     https://gist.github.com/RichardGale/6e2b74bc42b3005e08397236e4be0fd0

// ImGui BFFX binding
// In this binding, ImTextureID is used to store an OpenGL 'GLuint' texture
// identifier. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See
// main.cpp for an example of using this. If you use this binding you'll need to
// call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(),
// ImGui::Render() and ImGui_ImplXXXX_Shutdown(). If you are new to ImGui, see
// examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#include "imgui_impl_bgfx.h"
#include <imgui.h>

#include "bgfx/bgfx.h"
#include "bgfx/embedded_shader.h"
#include "bx/math.h"
#include "bx/timer.h"

struct RendererUserData
{
    uint8_t view = 0;
    uint8_t viewportViews = 1;
    bgfx::ProgramHandle shaderModule = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle textureAttributeLocation = BGFX_INVALID_HANDLE;
    bgfx::VertexLayout vertexLayout;

    int msaaSamples = 1;
    bool bUsingVSync = true;
};

bool ImGui_Implbgfx_UpdateTexture(ImTextureData* tex) noexcept;

void ImGui_Implbgfx_RenderDrawLists(ImDrawData* draw_data) noexcept
{
    const auto* backendData = static_cast<RendererUserData*>(ImGui::GetIO().BackendRendererUserData);

    const int fb_width  = static_cast<int>(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    const int fb_height = static_cast<int>(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;

    if (draw_data->Textures)
        for (ImTextureData* tex : *draw_data->Textures)
            if (tex->Status != ImTextureStatus_OK)
                ImGui_Implbgfx_UpdateTexture(tex);

    const bgfx::Caps* caps = bgfx::getCaps();

    const float L = draw_data->DisplayPos.x;
    const float T = draw_data->DisplayPos.y;
    const float R = L + draw_data->DisplaySize.x;
    const float B = T + draw_data->DisplaySize.y;

    float ortho[16];
    bx::mtxOrtho(ortho, L, R, B, T, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);

    bgfx::setViewTransform(backendData->view, nullptr, ortho);
    bgfx::setViewRect(backendData->view, 0, 0, static_cast<uint16_t>(fb_width), static_cast<uint16_t>(fb_height));

    constexpr uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA,BGFX_STATE_BLEND_INV_SRC_ALPHA);

    const ImVec2 clip_off   = draw_data->DisplayPos;
    const ImVec2 clip_scale = draw_data->FramebufferScale;

    for (int n = 0; n < draw_data->CmdListsCount; ++n)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];

        const auto vtx_count = static_cast<uint32_t>(cmd_list->VtxBuffer.size());
        const auto idx_count = static_cast<uint32_t>(cmd_list->IdxBuffer.size());

        // Allocate transient buffers (skip list if not enough space)
        bgfx::TransientVertexBuffer tvb;
        bgfx::TransientIndexBuffer  tib;
        if ((vtx_count != bgfx::getAvailTransientVertexBuffer(vtx_count, backendData->vertexLayout)) || (idx_count != bgfx::getAvailTransientIndexBuffer(idx_count)))
            break;

        bgfx::allocTransientVertexBuffer(&tvb, vtx_count, backendData->vertexLayout);
        bgfx::allocTransientIndexBuffer(&tib, idx_count);

        // Copy ImGui vertices / indices verbatim
        memcpy(tvb.data, cmd_list->VtxBuffer.begin(), vtx_count * sizeof(ImDrawVert));
        memcpy(tib.data, cmd_list->IdxBuffer.begin(), idx_count * sizeof(ImDrawIdx));

        // Walk the draw-commands for this list
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; ++cmd_i)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
                continue;
            }

            ImVec2 clip_min(
                (pcmd->ClipRect.x - clip_off.x) * clip_scale.x,
                (pcmd->ClipRect.y - clip_off.y) * clip_scale.y
            );
            ImVec2 clip_max(
                (pcmd->ClipRect.z - clip_off.x) * clip_scale.x,
                (pcmd->ClipRect.w - clip_off.y) * clip_scale.y
            );

            if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                continue;

            const auto sc_x = static_cast<uint16_t>(bx::max(clip_min.x, 0.0f));
            const auto sc_y = static_cast<uint16_t>(bx::max(clip_min.y, 0.0f));
            bgfx::setScissor(sc_x, sc_y,
                static_cast<uint16_t>(bx::min(clip_max.x, 65535.0f) - sc_x),
                static_cast<uint16_t>(bx::min(clip_max.y, 65535.0f) - sc_y)
            );

            // Bind state & resources and draw
            bgfx::setState(state);

            const bgfx::TextureHandle th = { static_cast<uint16_t>((static_cast<intptr_t>(pcmd->TexRef.GetTexID()) & 0xffff)) };
            bgfx::setTexture(0, backendData->textureAttributeLocation, th);

            bgfx::setVertexBuffer(0, &tvb, pcmd->VtxOffset, vtx_count);
            bgfx::setIndexBuffer(&tib, pcmd->IdxOffset, pcmd->ElemCount);
            bgfx::submit(backendData->view, backendData->shaderModule);
        }
    }
    if (draw_data->CmdListsCount == 0)
        bgfx::touch(backendData->view);
}

static void uploadRect(ImTextureData* texture, const int x, const int y, const int w, const int h) noexcept
{
    if (w <= 0 || h <= 0)
        return;

    const void* src = texture->GetPixelsAt(x, y);
    const bgfx::Memory* mem = bgfx::makeRef(src, texture->GetPitch() * h);

    bgfx::updateTexture2D(
        { static_cast<uint16_t>(texture->GetTexID()) },
        0,
        0,
        static_cast<uint16_t>(x),
        static_cast<uint16_t>(y),
        static_cast<uint16_t>(w),
        static_cast<uint16_t>(h),
        mem,
        static_cast<uint16_t>(texture->GetPitch())
    );
}

bool ImGui_Implbgfx_UpdateTexture(ImTextureData* tex) noexcept
{
    if (tex->Status == ImTextureStatus_WantCreate)
    {
        const auto texture = bgfx::createTexture2D(
            static_cast<uint16_t>(tex->Width),
            static_cast<uint16_t>(tex->Height),
            false,
            1,
            bgfx::TextureFormat::RGBA8,
            0,
            nullptr
        );

        bgfx::updateTexture2D(
            texture,
            0,
            0,
            0,
            0,
            static_cast<uint16_t>(tex->Width),
            static_cast<uint16_t>(tex->Height),
            bgfx::copy(tex->Pixels, tex->BytesPerPixel * tex->Width * tex->Height),
            tex->GetPitch()
        );

        tex->SetTexID(texture.idx);
        tex->SetStatus(ImTextureStatus_OK);
        return true;
    }
    if (tex->Status == ImTextureStatus_WantUpdates)
    {
        const bgfx::TextureHandle handle{ static_cast<uint16_t>(tex->GetTexID()) };
        if (!bgfx::isValid(handle))
            return false;

        uploadRect(tex, tex->UpdateRect.x, tex->UpdateRect.y, tex->UpdateRect.w, tex->UpdateRect.h);
        for (const auto& a : tex->Updates)
            uploadRect(tex, a.x, a.y, a.w, a.h);

        tex->SetStatus(ImTextureStatus_OK);
        return true;
    }
    if (tex->Status == ImTextureStatus_WantDestroy && tex->UnusedFrames > 0)
    {
        const bgfx::TextureHandle h{ static_cast<uint16_t>(tex->GetTexID()) };
        if (bgfx::isValid(h))
            bgfx::destroy(h);
        tex->SetTexID(ImTextureID_Invalid);
        tex->SetStatus(ImTextureStatus_Destroyed);
        return true;
    }
    return false;
}

#include "fs_ocornut_imgui.bin.h"
#include "vs_ocornut_imgui.bin.h"

static const bgfx::EmbeddedShader s_embeddedShaders[] =
{
    BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
    BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
    BGFX_EMBEDDED_SHADER_END()
};

struct ImGuiBGFXViewportData
{
    bgfx::FrameBufferHandle fb = BGFX_INVALID_HANDLE;
    bgfx::ViewId viewId = 0xff;
};

static void ImGui_Implbgfx_CreateWindow(ImGuiViewport* viewport) noexcept
{
    auto* vd = IM_NEW(ImGuiBGFXViewportData)();
    auto* userData = static_cast<RendererUserData*>(ImGui::GetIO().BackendRendererUserData);
    vd->viewId = userData->viewportViews++;

    vd->fb  = bgfx::createFrameBuffer(
        viewport->PlatformHandleRaw,
        static_cast<uint16_t>(viewport->Size.x * viewport->DrawData->FramebufferScale.x),
        static_cast<uint16_t>(viewport->Size.y * viewport->DrawData->FramebufferScale.y),
        bgfx::TextureFormat::RGBA8
    );

    viewport->RendererUserData = vd;
}

static void ImGui_Implbgfx_DestroyWindow(ImGuiViewport* viewport) noexcept
{
    if (auto* vd = static_cast<ImGuiBGFXViewportData*>(viewport->RendererUserData))
    {
        if (bgfx::isValid(vd->fb))
            bgfx::destroy(vd->fb);
        IM_DELETE(vd);
    }
    viewport->RendererUserData = nullptr;
}

static void ImGui_Implbgfx_RenderWindow(ImGuiViewport* viewport, void* renderArg) noexcept
{
    const auto* vd = static_cast<ImGuiBGFXViewportData*>(viewport->RendererUserData);

    const bgfx::ViewId viewId = vd ? vd->viewId : 0;
    const bgfx::FrameBufferHandle fb = vd ? vd->fb : bgfx::FrameBufferHandle(BGFX_INVALID_HANDLE);

    const auto w = static_cast<uint16_t>(viewport->Size.x * viewport->DrawData->FramebufferScale.x);
    const auto h = static_cast<uint16_t>(viewport->Size.y * viewport->DrawData->FramebufferScale.y);

    bgfx::setViewFrameBuffer(viewId, fb);
    bgfx::setViewRect(viewId, 0, 0, w, h);
    bgfx::touch(viewId);

    auto* data = static_cast<RendererUserData*>(ImGui::GetIO().BackendRendererUserData);

    const bgfx::ViewId prev = data->view;
    data->view = viewId;
    ImGui_Implbgfx_RenderDrawLists(viewport->DrawData);
    data->view = prev;
}

static void ImGui_Implbgfx_SwapBuffers(ImGuiViewport* viewport, void* renderArg) noexcept
{
    (void)viewport;
    (void)renderArg;
}

static void ImGui_Implbgfx_SetWindowSize(ImGuiViewport* viewport, const ImVec2 size) noexcept
{
    if (auto* vd = static_cast<ImGuiBGFXViewportData*>(viewport->RendererUserData))
    {
        if (bgfx::isValid(vd->fb))
            bgfx::destroy(vd->fb);

        auto* userData = static_cast<RendererUserData*>(ImGui::GetIO().BackendRendererUserData);
        vd->viewId = userData->viewportViews++;

        vd->fb = bgfx::createFrameBuffer(
            viewport->PlatformHandleRaw,
            static_cast<uint16_t>(size.x * viewport->DrawData->FramebufferScale.x),
            static_cast<uint16_t>(size.y * viewport->DrawData->FramebufferScale.y),
            bgfx::TextureFormat::RGBA8
        );
        viewport->RendererUserData = vd;
    }
    else
        bgfx::reset(static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y), ImGui_Implbgfx_GetResetFlags());
}

static void ImGui_Implbgfx_InitMultiViewportSupport() noexcept
{
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    platform_io.Renderer_CreateWindow = ImGui_Implbgfx_CreateWindow;
    platform_io.Renderer_DestroyWindow = ImGui_Implbgfx_DestroyWindow;
    platform_io.Renderer_SetWindowSize = ImGui_Implbgfx_SetWindowSize;
    platform_io.Renderer_RenderWindow = ImGui_Implbgfx_RenderWindow;
    platform_io.Renderer_SwapBuffers = ImGui_Implbgfx_SwapBuffers;
}

bool ImGui_Implbgfx_CreateDeviceObjects() noexcept
{
    const bgfx::RendererType::Enum type = bgfx::getRendererType();
    auto* data = static_cast<RendererUserData*>(ImGui::GetIO().BackendRendererUserData);

    data->shaderModule = bgfx::createProgram(
        bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_ocornut_imgui"),
        bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_ocornut_imgui"),
        true
    );

    data->vertexLayout
        .begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();

    data->textureAttributeLocation = bgfx::createUniform("g_AttribLocationTex", bgfx::UniformType::Sampler);

    return true;
}

void ImGui_Implbgfx_InvalidateDeviceObjects() noexcept
{
    for (const auto& a : ImGui::GetPlatformIO().Textures)
    {
        const bgfx::TextureHandle handle{ static_cast<uint16_t>(a->GetTexID()) };
        if (bgfx::isValid(handle))
            bgfx::destroy(handle);
    }

    const auto* data = static_cast<RendererUserData*>(ImGui::GetIO().BackendRendererUserData);
    bgfx::destroy(data->textureAttributeLocation);
    bgfx::destroy(data->shaderModule);
}

void ImGui_Implbgfx_Init(const int view, const int msaaSamples, const bool bUsingVSync) noexcept
{
    static RendererUserData data
    {
        .view = static_cast<uint8_t>(view & 0xff),
        .viewportViews = static_cast<uint8_t>(static_cast<uint8_t>(view & 0xff) + 1),
        .msaaSamples = msaaSamples,
        .bUsingVSync = bUsingVSync
    };

    auto& io = ImGui::GetIO();
    io.BackendRendererName = "imgui_impl_bgfx";
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
    io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;
    io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
    io.BackendRendererUserData = &data;
}

void ImGui_Implbgfx_Shutdown() noexcept
{
    ImGui_Implbgfx_InvalidateDeviceObjects();
    ImGui::GetIO().BackendRendererUserData = nullptr;
}

void ImGui_Implbgfx_NewFrame() noexcept
{
    static bool bFirst = true;
    if (bFirst)
    {
        ImGui_Implbgfx_CreateDeviceObjects();
        ImGui_Implbgfx_InitMultiViewportSupport();
        bFirst = false;
    }
}

int ImGui_Implbgfx_GetResetFlags() noexcept
{
    uint32_t flags = 0;
    const auto* data = static_cast<RendererUserData*>(ImGui::GetIO().BackendRendererUserData);

    if (data->bUsingVSync)
        flags = BGFX_RESET_VSYNC;

    if (data->msaaSamples >= 16)
        flags |= BGFX_RESET_MSAA_X16;
    else if (data->msaaSamples >= 8)
        flags |= BGFX_RESET_MSAA_X8;
    else if (data->msaaSamples >= 4)
        flags |= BGFX_RESET_MSAA_X4;
    else if (data->msaaSamples >= 2)
        flags |= BGFX_RESET_MSAA_X2;
    return flags;
}
