#pragma once

#include "engine_export.h"

#include <ComponentBase.h>

class ENGINE_EXPORT SpriteRenderer : public ComponentBase<SpriteRenderer> {
public:
    SpriteRenderer() = default;
    ~SpriteRenderer() override = default;

    std::string texturePath;

#ifdef ENABLE_IMGUI
    void onInspectorGUI() override;
#endif
};

inline constexpr std::array<StringField<SpriteRenderer>, 1> kSpriteRendererStrings = {{
    { .label = "TexturePath", .member = &SpriteRenderer::texturePath }
}};

template <>
constexpr Property<SpriteRenderer> buildMetadata<SpriteRenderer>() {
    return Property<SpriteRenderer> {
        .name = "SpriteRenderer",
        .strings = std::span{kSpriteRendererStrings}
    };
}

