#pragma once

#include <glm/glm.hpp>
#include <bgfx/bgfx.h>

#include "engine_export.h"

#include <ComponentBase.h>

class ENGINE_EXPORT Camera : public ComponentBase<Camera> {
public:
    float fovDeg = 60.0f;
    float zNear = 0.1f;
    float zFar = 100.0f;

    explicit Camera() = default;
    ~Camera() override = default;

    // Transforms world space -> camera space.
    glm::mat4 view() const;

    // Transforms camera space -> clip space
    glm::mat4 proj() const;

    void render(GameObject& owner) override {};
    void update(GameObject& owner, float deltaTime) override {}

#ifdef ENABLE_IMGUI
    void onInspectorGUI() override;
#endif
};

inline constexpr NumericField<Camera, float> kCameraFloats[] = {
    { .label = "FOV", .member = &Camera::fovDeg, .step = 1.0f, .min = 0.0f, .max = 180.0f },
    { .label = "ZNear", .member = &Camera::zNear, .step = 1.0f, .min = 0.1f, .max = 100.0f },
    { .label = "ZFar", .member = &Camera::zFar, .step = 1.0f, .min = 0.1f, .max = 100.0f }
};

template <>
constexpr Property<Camera> buildMetadata<Camera>() {
    return Property<Camera> {
        .name = "Camera",
        .floats = std::span{kCameraFloats}
    };
}
