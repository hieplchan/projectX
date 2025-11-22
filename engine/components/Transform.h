#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine_export.h"

#include <ComponentBase.h>

class ENGINE_EXPORT Transform : public ComponentBase<Transform> {
public:

    explicit Transform() = default;
    ~Transform() override = default;

    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 rotation{ 0.0f, 0.0f, 0.0f }; // Euler angles in degrees
    glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

    // Compute the transformation matrix
    glm::mat4 matrix() const;

    void update(GameObject& owner, float deltaTime) override {}
    void render(GameObject& owner) override {}

#if defined(ENABLE_IMGUI)
    void onInspectorGUI() override;
#endif
};

inline constexpr Vec3Field<Transform> kTransformVec3s[] = {
    { .label = "Position", .vec3 = &Transform::position, .step = 0.1f, .min = -1000.0f, .max = 1000.0f },
    { .label = "Rotation", .vec3 = &Transform::rotation, .step = 0.1f, .min = 0.0f, .max = 360.0f },
    { .label = "Scale", .vec3 = &Transform::scale, .step = 0.1f, .min = 0.0f, .max = 1000.0f },
};

template <>
constexpr Property<Transform> buildMetadata<Transform>() {
    return Property<Transform> {
        .name = "Transform",
        .vec3s = std::span{kTransformVec3s}
    };
}
