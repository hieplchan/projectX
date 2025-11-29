#include <common/common_include.h>

#include "Transform.h"
#include "QuadRenderer.h"

#if defined(ENABLE_IMGUI)
#include <InspectorRenderer.h>

void QuadRenderer::onInspectorGUI() {
    Inspector::drawFromProperty<QuadRenderer>(this, reflect<QuadRenderer>());
}
#endif

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

    constexpr std::array<PosColorVertex, 4> s_cubeVertices
    {
        PosColorVertex{0.5f, 0.5f, 0.0f, 0xff0000ff},
        PosColorVertex{0.5f, -0.5f, 0.0f, 0xff0000ff},
        PosColorVertex{-0.5f, -0.5f, 0.0f, 0xff00ff00},
        PosColorVertex{-0.5f, 0.5f, 0.0f, 0xff00ff00}
    };

    constexpr std::array<uint16_t, 6> s_cubeTriList
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
#pragma region Load Shaders
    PosColorVertex::init();
    m_vb = bgfx::createVertexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeVertices.data(), sizeof(s_cubeVertices)),
        PosColorVertex::ms_decl);

    m_ib = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeTriList.data(), sizeof(s_cubeTriList)));
    bgfx::ShaderHandle vsh = loadShader("vs_quad");
    bgfx::ShaderHandle fsh = loadShader("fs_quad");
    m_prog = bgfx::createProgram(vsh, fsh, true);
#pragma endregion
}

QuadRenderer::QuadRenderer() : QuadRenderer(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}) { }

QuadRenderer::~QuadRenderer() {
    BGFX_SAFE_DESTROY_HANDLE(m_vb);
    BGFX_SAFE_DESTROY_HANDLE(m_ib);
    BGFX_SAFE_DESTROY_HANDLE(m_prog);
}

void QuadRenderer::render(GameObject& owner) {
    const auto* tf = getOwner()->getComponent<Transform>();
    const glm::mat4 model = tf ? tf->matrix() : glm::mat4(1.0f);

    bgfx::setTransform(glm::value_ptr(model));

    //// Set vertex and index buffer.
    bgfx::setVertexBuffer(0, m_vb);
    bgfx::setIndexBuffer(m_ib);

    //// Set render states.
    bgfx::setState(BGFX_STATE_DEFAULT);

    //// Submit primitive for rendering to view 0.
    bgfx::submit(getCtxSettings().viewIds.world, m_prog);
}
