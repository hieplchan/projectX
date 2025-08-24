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

    glm::mat4 view(uint32_t viewPortW, uint32_t viewPortH) const;
    glm::mat4 proj(uint32_t viewPortW, uint32_t viewPortH) const;

    void render(GameObject& owner) override {};
    void update(GameObject& owner, float deltaTime) override {}
};
