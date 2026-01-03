#pragma once

#include <bgfx/bgfx.h>

#include "engine_export.h"

#include <ComponentBase.h>

struct Pos2D {
    float x;
    float y;
};

class ENGINE_EXPORT QuadRenderer final : public ComponentBase<QuadRenderer> {
X_COMPONENT(QuadRenderer)

public:
    [[field]] glm::vec4 color{ 1.0f };

    explicit QuadRenderer();
    ~QuadRenderer() override;

    void render(GameObject& owner) override;

private:

    static constexpr std::string_view kVertexShaderName = "vs_quad.bin";
    static constexpr std::string_view kFragmentShaderName = "fs_quad.bin";

    bgfx::VertexBufferHandle    m_hVertBuf{ bgfx::kInvalidHandle };
    bgfx::IndexBufferHandle     m_hIndexBuf{ bgfx::kInvalidHandle };
    bgfx::ProgramHandle         m_hProg{ bgfx::kInvalidHandle };
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
