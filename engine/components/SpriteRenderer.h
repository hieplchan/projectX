#pragma once

#include "engine_export.h"

#include <ComponentBase.h>

class ENGINE_EXPORT SpriteRenderer : public ComponentBase<SpriteRenderer> {
public:
    void onDeserialized() override;

    std::string texFilePath;
    bgfx::TextureHandle texHandle{ bgfx::kInvalidHandle };

#ifdef ENABLE_IMGUI
    void onInspectorGUI() override;
#endif
};

inline constexpr std::array<StringField<SpriteRenderer>, 1> kSpriteRendererStrings = {{
    { .label = "TexturePath", .member = &SpriteRenderer::texFilePath }
}};

template <>
constexpr Property<SpriteRenderer> buildMetadata<SpriteRenderer>() {
    return Property<SpriteRenderer> {
        .name = "SpriteRenderer",
        .strings = std::span{kSpriteRendererStrings}
    };
}

