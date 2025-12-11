#pragma once
#include <bgfx/bgfx.h>

#include "engine_export.h"

#include <ComponentBase.h>

class ENGINE_EXPORT SpriteRenderer : public ComponentBase<SpriteRenderer> {
public:
    explicit SpriteRenderer();
    ~SpriteRenderer() override;

    void onDeserialized() override;

    std::string texFilePath;
    glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

    bgfx::TextureHandle texHandle{ bgfx::kInvalidHandle };
    bgfx::ProgramHandle progHandle{ bgfx::kInvalidHandle };

    static constexpr std::string_view kVertexShaderName = "vs_sprite.bin";
    static constexpr std::string_view kFragmentShaderName = "fs_sprite.bin";

#ifdef ENABLE_IMGUI
    void onInspectorGUI() override;
#endif
};

inline constexpr std::array<StringField<SpriteRenderer>, 1> kSpriteRendererStrings = {{
    { .label = "TexturePath", .member = &SpriteRenderer::texFilePath }
}};
inline constexpr std::array<ColorField<SpriteRenderer>, 1> kSpriteRendererColors = {{
    { .label = "Tint", .color = &SpriteRenderer::color}
}};

template <>
constexpr Property<SpriteRenderer> buildMetadata<SpriteRenderer>() {
    return Property<SpriteRenderer> {
        .name = "SpriteRenderer",
        .strings = std::span{kSpriteRendererStrings},
        .colors = std::span{kSpriteRendererColors}
    };
}

