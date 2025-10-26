#include <common/common_include.h>

#include "Transform.h"
#include "QuadRenderer.h"

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

QuadRenderer::QuadRenderer(const glm::vec4& color) : color(color) {
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
    auto* tf = m_owner->getComponent<Transform>();
    const glm::mat4 model = tf ? tf->matrix() : glm::mat4(1.0f);

    bgfx::setTransform(glm::value_ptr(model));

    //// Set vertex and index buffer.
    bgfx::setVertexBuffer(0, m_vb);
    bgfx::setIndexBuffer(m_ib);

    //// Set render states.
    bgfx::setState(BGFX_STATE_DEFAULT);

    //// Submit primitive for rendering to view 0.
    bgfx::submit(m_ctx->window.viewIds.world, m_prog);
}
