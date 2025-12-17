#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine_export.h"

#include <ComponentBase.h>

class ENGINE_EXPORT Transform final : public ComponentBase<Transform> {
X_COMPONENT(Transform)

public:
    [[field]] glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    [[field]] glm::vec3 rotation{ 0.0f, 0.0f, 0.0f }; // Euler angles in degrees
    [[field]] glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

    // Transformation matrix
    glm::mat4 matrix() const {
            glm::mat4 M(1.0f);
        M = glm::translate(M, position);
        M = glm::rotate(M, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        M = glm::rotate(M, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        M = glm::rotate(M, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        M = glm::scale(M, scale);
        return M;
    }
};

inline constexpr std::array<Vec3Field<Transform>, 3> kTransformVec3s = {{
    { .label = "Position", .vec3 = &Transform::position, .step = 0.1f, .min = -1000.0f, .max = 1000.0f },
    { .label = "Rotation", .vec3 = &Transform::rotation, .step = 0.1f, .min = 0.0f, .max = 360.0f },
    { .label = "Scale", .vec3 = &Transform::scale, .step = 0.1f, .min = 0.0f, .max = 1000.0f },
}};

template <>
constexpr Property<Transform> buildMetadata<Transform>() {
    return Property<Transform> {
        .name = "Transform",
        .vec3s = std::span{kTransformVec3s}
    };
}
