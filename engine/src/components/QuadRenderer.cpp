
// #include "Renderer.h"

#include "common/common_include.h"

#include "components/Transform.h"
#include "components/QuadRenderer.h"

namespace {
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

    // Load the flat-colored quad shader program
    bgfx::ProgramHandle quadProgram() {
        static const bgfx::ProgramHandle p = loadProgram("vs_quad", "fs_quad");
        return p;
    }

    // Uniform for the quad color
    bgfx::UniformHandle quadColorUniform() {
        static const bgfx::UniformHandle u = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
        return u;
    }
}

QuadRenderer::QuadRenderer(const glm::vec4& color) : m_color(color) {
    LOG_INFO("constructing...");

#pragma region Load Shaders
    PosColorVertex::init();
    m_vb = bgfx::createVertexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
        PosColorVertex::ms_decl);

    m_ib = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
    bgfx::ShaderHandle vsh = loadShader("vs_quad");
    bgfx::ShaderHandle fsh = loadShader("fs_quad");
    m_prog = bgfx::createProgram(vsh, fsh, true);
#pragma endregion
}

QuadRenderer::~QuadRenderer() {
    BGFX_SAFE_DESTROY_HANDLE(m_vb);
    BGFX_SAFE_DESTROY_HANDLE(m_ib);
    BGFX_SAFE_DESTROY_HANDLE(m_prog);
    LOG_INFO("destructed");
}

void QuadRenderer::render(GameObject& owner) {
    // LOG_INFO("QuadRenderer::render()");
    // const auto* transform = owner.getComponent<Transform>();
    // glm::mat4 mtx = transform ? transform->matrix() : glm::mat4(1.0f);

    // // set transform and color
    // bgfx::setTransform(&mtx[0][0]);
    // bgfx::setVertexBuffer(0, m_vb);
    // bgfx::setIndexBuffer(m_ib);

    // // Set color
    // bgfx::setUniform(quadColorUniform(), &m_color[0]);

    // // Submit the draw call
    // bgfx::submit(m_windowSettings.viewId, quadProgram());

#pragma region Camera
    const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
    const bx::Vec3 eye = { 0.0f, 0.0f, 10.0f };

    // Set view and projection matrix for view 0.
    float view[16];
    bx::mtxLookAt(view, eye, at);

    float proj[16];
    bx::mtxProj(proj,
        60.0f,
        float(m_windowSettings.width) / float(m_windowSettings.height),
        0.1f, 100.0f,
        bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(0, view, proj);

    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0,
        m_windowSettings.width,
        m_windowSettings.height);

    bgfx::touch(0);
#pragma endregion

#pragma region Object
    float mtx[16];
    bx::mtxRotateY(mtx, 0.0f);

    // position x,y,z
    mtx[12] = 0.0f;
    mtx[13] = 0.0f;
    mtx[14] = 0.0f;

    //// Set model matrix for rendering.
    bgfx::setTransform(mtx);

    //// Set vertex and index buffer.
    bgfx::setVertexBuffer(0, m_vb);
    bgfx::setIndexBuffer(m_ib);

    //// Set render states.
    bgfx::setState(BGFX_STATE_DEFAULT);

    //// Submit primitive for rendering to view 0.
    bgfx::submit(0, m_prog);
#pragma endregion

    // bgfx::frame();
}
