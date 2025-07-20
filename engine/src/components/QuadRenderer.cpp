#include "common/common_include.h"

#include "Transform.h"
#include "Renderer.h"
#include "QuadRenderer.h"

namespace {
    const bgfx::VertexLayout& getQuadLayout() {
        static const bgfx::VertexLayout layout = []() {
            bgfx::VertexLayout tmp;
            tmp.begin()
                .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
                .end();
            return tmp;
        }();
        return layout;
    }

    // const bgfx::ProgramHandle quadPrograme() {
    //     static const bgfx::ProgramHandle p = loadPro
    // }
}

QuadRenderer::QuadRenderer(const glm::vec4& color) : m_color(color) 
{
    Pos2D vertices[4] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
    };

    // Create index buffer for the quad
    // Two triangles: (0, 1, 2) and (0, 2, 3)
    uint16_t indices[6] = { 0, 1, 2, 0, 2, 3 };

    m_vb = bgfx::createVertexBuffer(
        bgfx::makeRef(vertices, sizeof(vertices)),
        getQuadLayout()
    );

    m_ib = bgfx::createIndexBuffer(
        bgfx::makeRef(indices, sizeof(indices))
    );
}

QuadRenderer::~QuadRenderer() {
    BGFX_SAFE_DESTROY_HANDLE(m_vb);
    BGFX_SAFE_DESTROY_HANDLE(m_ib);
}

void QuadRenderer::render(GameObject& owner)
{
    const auto* transform = owner.getComponent<Transform>();
    glm::mat4 mtx = transform ? transform->matrix() : glm::mat4(1.0f);

    bgfx::setTransform(&mtx[0][0]);
    bgfx::setVertexBuffer(0, m_vb);
    bgfx::setIndexBuffer(m_ib);
    // bgfx::setTexture(0, bgfx::UniformHandle(), m_tex);
    // bgfx::submit(m_windowSettings.viewId, bgfx::ProgramHandle());
}