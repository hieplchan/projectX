#pragma once
#include <bgfx/bgfx.h>

#include "engine_export.h"

#include <ComponentBase.h>

class ENGINE_EXPORT SpriteRenderer final : public ComponentBase<SpriteRenderer> {
public:
    [[field]] std::string texFilePath;
    [[field]] glm::vec4 tint{ 1.0f };

    explicit SpriteRenderer();
    ~SpriteRenderer() override;

    void onDeserialized() override;

private:
    static constexpr std::string_view kVertexShaderName = "vs_sprite.bin";
    static constexpr std::string_view kFragmentShaderName = "fs_sprite.bin";

    bgfx::UniformHandle m_uTint{ bgfx::kInvalidHandle };
    bgfx::UniformHandle m_uTexSampler{ bgfx::kInvalidHandle };

    bgfx::VertexBufferHandle    m_hVertBuf{ bgfx::kInvalidHandle };
    bgfx::IndexBufferHandle     m_hIndexBuf{ bgfx::kInvalidHandle };
    bgfx::TextureHandle         m_hTex{ bgfx::kInvalidHandle };
    bgfx::ProgramHandle         m_hProg{ bgfx::kInvalidHandle };

#ifdef ENABLE_IMGUI
public:
    void onInspectorGUI() override;
#endif
};

inline constexpr std::array<StringField<SpriteRenderer>, 1> kSpriteRendererStrings = {{
    { .label = "TexturePath", .member = &SpriteRenderer::texFilePath }
}};
inline constexpr std::array<ColorField<SpriteRenderer>, 1> kSpriteRendererColors = {{
    { .label = "Tint", .color = &SpriteRenderer::tint}
}};

template <>
constexpr Property<SpriteRenderer> buildMetadata<SpriteRenderer>() {
    return Property<SpriteRenderer> {
        .name = "SpriteRenderer",
        .strings = std::span{kSpriteRendererStrings},
        .colors = std::span{kSpriteRendererColors}
    };
}

