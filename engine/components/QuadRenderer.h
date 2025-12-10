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

    bgfx::VertexBufferHandle vbHandle{ bgfx::kInvalidHandle };
    bgfx::IndexBufferHandle ibHandle{ bgfx::kInvalidHandle };
    bgfx::ProgramHandle progHandle{ bgfx::kInvalidHandle };

    static constexpr std::string_view kVertexShaderName = "vs_quad.bin";
    static constexpr std::string_view kFragmentShaderName = "fs_quad.bin";

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
