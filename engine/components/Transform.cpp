#include "common/common_include.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"

#if defined(ENABLE_IMGUI)
#include <imgui.h>

void Transform::onInspectorGUI() {
    ImGui::SeparatorText(inspectorName().data());
    ImGui::DragFloat3("Position", glm::value_ptr(position), 0.05f);
    ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.2f);
    ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.01f, 0.01f, 100.0f);
}
#endif

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
