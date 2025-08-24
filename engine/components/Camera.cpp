#include "common/common_include.h"

#include "Transform.h"
#include "Camera.h"

Camera::Camera() {
    LOG_INFO("constructing...");
}

Camera::~Camera() {
    LOG_INFO("destructed...");
}

glm::mat4 Camera::view(uint32_t viewPortW, uint32_t viewPortH) const {
    if (auto* tf = owner()->getComponent<Transform>()) {

    }
    return glm::mat4(1.0f);
}

glm::mat4 Camera::proj(uint32_t viewPortW, uint32_t viewPortH) const {
    return glm::mat4(1.0f);
}
