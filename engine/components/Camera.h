#pragma once

#include <glm/glm.hpp>
#include <bgfx/bgfx.h>

#include "engine_export.h"

#include "GameObject.h"
#include <Component.h>

class ENGINE_EXPORT Camera : public Component {
public:
    float fovDeg = 60.0f;
    float zNear = 0.1f;
    float zFar = 100.0f;

    explicit Camera();
    ~Camera() override;

    // Transforms world space -> camera space.
    glm::mat4 view() const;

    // Transforms camera space -> clip space
    glm::mat4 proj() const;

    void render(GameObject& owner) override {};
    void update(GameObject& owner, float deltaTime) override {}
};
