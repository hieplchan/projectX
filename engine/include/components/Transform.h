#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Component.h"

class Transform : public Component {
public:
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec3 rotation{0.0f, 0.0f, 0.0f}; // Euler angles in degrees
    glm::vec3 scale{1.0f, 1.0f, 1.0f};

    // Compute the transformation matrix
    glm::mat4 matrix() const {
        glm::mat4 M(1.0f);
        M = glm::translate(M, position);
        M = glm::rotate(M, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        M = glm::rotate(M, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        M = glm::rotate(M, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        M = glm::scale(M, scale);
        return M;
    };

    void update(GameObject& owner, float deltaTime) override {}
    void render(GameObject& owner) override {}
};