#include "common/common_include.h"

#include "Transform.h"
#include "Camera.h"

// GL Camera: https://learnopengl.com/Getting-started/Camera
glm::mat4 Camera::view() const {
    if (const auto* tf = owner()->getComponent<Transform>()) {
        const glm::vec3 eye = tf->position;
        const auto at = eye + glm::vec3(0.0f, 0.0f, 10.0f);
        const auto up = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::mat4 view = glm::lookAt(eye, at, up);
        return view;
    }

    // Fallback if no Transform on the camera object
    return glm::lookAt(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0));
}

glm::mat4 Camera::proj() const {
    const float aspect = (getCtxSettings().height > 0)
        ? static_cast<float>(getCtxSettings().width) / static_cast<float>(getCtxSettings().height)
        : 1.0f;
    return glm::perspective( // Perspective projection matrix
        glm::radians(fovDeg), // Vertical field of view
        aspect, // keep image from being stretch
        zNear, zFar
    );
}
