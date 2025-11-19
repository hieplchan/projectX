#pragma once

#include <bgfx/bgfx.h>

#include "engine_export.h"

#include <ComponentBase.h>

struct Pos2D {
    float x, y;
};

class ENGINE_EXPORT QuadRenderer : public ComponentBase<QuadRenderer> {
public:
    glm::vec4 color;

    explicit QuadRenderer(const glm::vec4& color);
    ~QuadRenderer() override;

    void render(GameObject& owner) override;
    void update(GameObject& owner, float deltaTime) override {}

#if defined(ENABLE_IMGUI)
    void onInspectorGUI() override;
#endif

private:
    bgfx::VertexBufferHandle m_vb{ bgfx::kInvalidHandle };
    bgfx::IndexBufferHandle m_ib{ bgfx::kInvalidHandle };
    bgfx::ProgramHandle m_prog{ bgfx::kInvalidHandle };
};

inline constexpr ColorField<QuadRenderer> kQuadRendererColors[] = {
    { .label = "Color", .color = &QuadRenderer::color}
};

template <>
constexpr Property<QuadRenderer> buildMetadata<QuadRenderer>() {
    return Property<QuadRenderer> {
        .name = "QuadRenderer",
        .colors = std::span{kQuadRendererColors}
    };
}
