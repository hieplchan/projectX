#include <bgfx_utils.h>

#include <common/common_include.h>
#include <GameObject.h>
#include "Logger.h"
#include "Transform.h"
#include "SpriteRenderer.h"

namespace {
struct PosUVVertex {
    float x, y, z;
    float u, v;

    static void init(bgfx::VertexLayout& layout) {
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }

    static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosUVVertex::ms_layout;

constexpr std::array<PosUVVertex, 4> kQuadVertices = {{
    PosUVVertex{-0.5f, -0.5f, 0.0f, 0.0f, 1.0f}, // bl
    PosUVVertex{-0.5f,  0.5f, 0.0f, 0.0f, 0.0f}, // tl
    PosUVVertex{ 0.5f,  0.5f, 0.0f, 1.0f, 0.0f}, // tr
    PosUVVertex{ 0.5f, -0.5f, 0.0f, 1.0f, 1.0f}  // br
}};

constexpr std::array<uint16_t, 6> kQuadIndices = {
    0, 1, 2,
    0, 2, 3
};

} // anonymous namespace

SpriteRenderer::SpriteRenderer() {
    PosUVVertex::init(PosUVVertex::ms_layout);

    m_hVertBuf = bgfx::createVertexBuffer(
        bgfx::makeRef(kQuadVertices.data(), sizeof(kQuadVertices)),
        PosUVVertex::ms_layout
    );

    m_hIndexBuf = bgfx::createIndexBuffer(
        bgfx::makeRef(kQuadIndices.data(), sizeof(kQuadIndices))
    );

    bgfx::ShaderHandle vsh = loadShader(kVertexShaderName);
    bgfx::ShaderHandle fsh = loadShader(kFragmentShaderName);
    m_hProg = bgfx::createProgram(vsh, fsh, true);
    if (!bgfx::isValid(m_hProg)) {
        LOG_ERROR("SpriteRenderer: Failed to create shader program");
    }

    m_uTint = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
    m_uTexSampler = bgfx::createUniform("s_tex0", bgfx::UniformType::Sampler);
}

SpriteRenderer::~SpriteRenderer() {
    BGFX_SAFE_DESTROY_HANDLE(m_uTint);
    BGFX_SAFE_DESTROY_HANDLE(m_uTexSampler);
    BGFX_SAFE_DESTROY_HANDLE(m_hProg);
    BGFX_SAFE_DESTROY_HANDLE(m_hTex);
    BGFX_SAFE_DESTROY_HANDLE(m_hVertBuf);
    BGFX_SAFE_DESTROY_HANDLE(m_hIndexBuf);
}

void SpriteRenderer::onDeserialized() {
    bgfx::TextureHandle tex = loadTexture(texFilePath);
    if (!bgfx::isValid(tex)) {
        LOG_ERROR("SpriteRenderer: Failed to load texture {}", texFilePath);
        return;
    }
    m_hTex = tex;
}

void SpriteRenderer::render(GameObject& owner) {
    const auto* tf = getOwner()->getComponent<Transform>();
    const glm::mat4 model = tf ? tf->matrix() : glm::mat4(1.0f);
    // bgfx::setTransform(glm::value_ptr(model));

    // apply pivot and size: quad is -0.5..0.5 so scale by size, translate to pivot
    glm::mat4 m = model;
    // translate by pivot in local quad space
    m = glm::translate(m, glm::vec3((0.5f - pivot.x) * size.x, (0.5f - pivot.y) * size.y, 0.0f));
    m = glm::scale(m, glm::vec3(size.x, size.y, 1.0f));

    float mtx[16];
    memcpy(mtx, glm::value_ptr(m), sizeof(mtx));
    bgfx::setTransform(mtx);

    //// Set vertex and index buffer.
    bgfx::setVertexBuffer(0, m_hVertBuf);
    bgfx::setIndexBuffer(m_hIndexBuf);

    // bind texture and tint
    bgfx::setTexture(0, m_uTexSampler, m_hTex);
    // bgfx::setUniform(m_uTint, glm::value_ptr(tint));

    // state (write rgb + alpha, alpha blend)
    // const uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA;
    bgfx::setState(BGFX_STATE_WRITE_RGB);

    // submit - use engine's world view id
    bgfx::submit(getCtxSettings().viewIds.world, m_hProg);
}
