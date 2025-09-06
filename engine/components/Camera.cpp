#include "common/common_include.h"

#include "Transform.h"
#include "Camera.h"

Camera::Camera() {
    LOG_INFO("constructing...");
}

Camera::~Camera() {
    LOG_INFO("destructed...");
}

// GL Camera: https://learnopengl.com/Getting-started/Camera
glm::mat4 Camera::view() const {
    if (auto* tf = owner()->getComponent<Transform>()) {
        const glm::vec3 eye = tf->position;
        const glm::vec3 at = eye + glm::vec3(0.0f, 0.0f, 10.0f);
        const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        /// TODO: add roll rotation
        glm::mat4 view = glm::lookAt(eye, at, up);
        return view;
    }

    // Fallback if no Transform on the camera object
    return glm::lookAt(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0));
}

glm::mat4 Camera::proj() const {
    const float aspect = (m_ctx->window.height > 0)
        ? static_cast<float>(m_ctx->window.width) / static_cast<float>(m_ctx->window.height)
        : 1.0f;
    return glm::perspective( // Perspective projection matrix
        glm::radians(fovDeg), // Vertical field of view
        aspect, // keep image from being stretch
        zNear, zFar
    );
}
