#pragma once

#include "engine_export.h"

#include <ComponentBase.h>

class ENGINE_EXPORT SpriteRenderer : public ComponentBase<SpriteRenderer> {
public:
    SpriteRenderer() = default;
    ~SpriteRenderer() override = default;

#ifdef ENABLE_IMGUI
    void onInspectorGUI() override;
#endif
};

template <>
constexpr Property<SpriteRenderer> buildMetadata<SpriteRenderer>() {
    return Property<SpriteRenderer> {
        .name = "SpriteRenderer",
    };
}

