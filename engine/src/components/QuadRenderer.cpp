
// #include "Renderer.h"

#include "common/common_include.h"

#include "components/Transform.h"
#include "components/QuadRenderer.h"

namespace {
    // Define the vertex layout for the quad renderer, position only
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

QuadRenderer::QuadRenderer(const glm::vec4& color) : m_color(color) 
{
    // Top-left, bottom-left, bottom-right, top-right
    Pos2D vertices[4] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
    };

    // Quad = 2 triangles: (0, 1, 2) and (0, 2, 3)
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

    // set transform and color
    bgfx::setTransform(&mtx[0][0]);
    bgfx::setVertexBuffer(0, m_vb);
    bgfx::setIndexBuffer(m_ib);

    // Set color
    bgfx::setUniform(quadColorUniform(), &m_color[0]);

    // Submit the draw call
    bgfx::submit(m_windowSettings.viewId, quadProgram());
}