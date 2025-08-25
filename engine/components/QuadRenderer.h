#pragma once

#include <bgfx/bgfx.h>

#include "engine_export.h"

#include "GameObject.h"
#include <Component.h>

struct Pos2D {
    float x, y;
};

class ENGINE_EXPORT QuadRenderer : public Component {
public:
    glm::vec4 color;

    explicit QuadRenderer(const glm::vec4& color);
    ~QuadRenderer() override;

    void render(GameObject& owner) override;
    void update(GameObject& owner, float deltaTime) override {}

private:
    bgfx::VertexBufferHandle m_vb{ bgfx::kInvalidHandle };
    bgfx::IndexBufferHandle m_ib{ bgfx::kInvalidHandle };
    bgfx::ProgramHandle m_prog{ bgfx::kInvalidHandle };
};
