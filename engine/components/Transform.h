#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine_export.h"

#include <Component.h>

class ENGINE_EXPORT Transform : public Component {
public:

    explicit Transform();
    ~Transform() override;

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
