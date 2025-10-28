#include "common/common_include.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"
#include <editor/Inspector.h>

namespace {
    template class ComponentBase<Transform>;
    template void Inspector::drawInspector<Transform>(Transform*);
}

Transform::Transform() {
    LOG_INFO("constructing...");
}

Transform::~Transform() {
    LOG_INFO("destructed...");
}

glm::mat4 Transform::matrix() const {
    glm::mat4 M(1.0f);
    M = glm::translate(M, position);
    M = glm::rotate(M, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    M = glm::rotate(M, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    M = glm::rotate(M, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    M = glm::scale(M, scale);
    return M;
};
