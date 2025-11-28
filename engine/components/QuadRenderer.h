#pragma once

#include <bgfx/bgfx.h>

#include "engine_export.h"

#include <ComponentBase.h>

struct Pos2D {
    float x;
    float y;
};

class ENGINE_EXPORT QuadRenderer : public ComponentBase<QuadRenderer> {
public:
    explicit QuadRenderer(const glm::vec4& color);
    explicit QuadRenderer();

    ~QuadRenderer() override;

    void render(GameObject& owner) override;

    glm::vec4 color;
    bgfx::VertexBufferHandle m_vb{ bgfx::kInvalidHandle };
    bgfx::IndexBufferHandle m_ib{ bgfx::kInvalidHandle };
    bgfx::ProgramHandle m_prog{ bgfx::kInvalidHandle };

#if defined(ENABLE_IMGUI)
    void onInspectorGUI() override;
#endif
};

inline constexpr std::array<ColorField<QuadRenderer>, 1> kQuadRendererColors = {{
    { .label = "Color", .color = &QuadRenderer::color}
}};

template <>
constexpr Property<QuadRenderer> buildMetadata<QuadRenderer>() {
    return Property<QuadRenderer> {
        .name = "QuadRenderer",
        .colors = std::span{kQuadRendererColors}
    };
}
