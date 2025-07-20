#pragma once

#include <glm/glm.hpp>
#include <bgfx/bgfx.h>

#include "GameObject.h"
#include "../Component.h"

struct Pos2D {
    float x, y;
};

class QuadRenderer : public Component {
public:
    explicit QuadRenderer(const glm::vec4& color);
    ~QuadRenderer() override;

    void render(GameObject& owner) override;

private:
    glm::vec4 m_color;

    bgfx::VertexBufferHandle m_vb{ bgfx::kInvalidHandle };
    bgfx::IndexBufferHandle m_ib{ bgfx::kInvalidHandle };
};