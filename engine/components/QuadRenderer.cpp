#include <common/common_include.h>

#include "Transform.h"
#include "QuadRenderer.h"

namespace {
    struct PosColorVertex
    {
        float x;
        float y;
        float z;
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

QuadRenderer::QuadRenderer() {
    PosColorVertex::init();
    m_hVertBuf = bgfx::createVertexBuffer(
        bgfx::makeRef(
            s_cubeVertices.data(),
            sizeof(s_cubeVertices)
        ),
        PosColorVertex::ms_decl
    );

    m_hIndexBuf = bgfx::createIndexBuffer(
        bgfx::makeRef(
            s_cubeTriList.data(),
            sizeof(s_cubeTriList)
        )
    );

    bgfx::ShaderHandle vsh = loadShader(kVertexShaderName);
    bgfx::ShaderHandle fsh = loadShader(kFragmentShaderName);
    m_hProg = bgfx::createProgram(vsh, fsh, true);
    if (!bgfx::isValid(m_hProg)) {
        LOG_ERROR("QuadRenderer: Failed to create shader program");
    }
}

QuadRenderer::~QuadRenderer() {
    BGFX_SAFE_DESTROY_HANDLE(m_hVertBuf);
    BGFX_SAFE_DESTROY_HANDLE(m_hIndexBuf);
    BGFX_SAFE_DESTROY_HANDLE(m_hProg);
}

void QuadRenderer::render(GameObject& owner) {
    const auto* tf = getOwner()->getComponent<Transform>();
    const glm::mat4 model = tf ? tf->matrix() : glm::mat4(1.0f);

    bgfx::setTransform(glm::value_ptr(model));

    //// Set vertex and index buffer.
    bgfx::setVertexBuffer(0, m_hVertBuf);
    bgfx::setIndexBuffer(m_hIndexBuf);

    //// Set render states.
    bgfx::setState(BGFX_STATE_DEFAULT);

    //// Submit primitive for rendering to view 0.
    bgfx::submit(getCtxSettings().viewIds.world, m_hProg);
}
