#include <common/common_include.h>
#include "GameObject.h"

#if defined(ENABLE_IMGUI)
#include <imgui.h>

void GameObject::onInspectorGUI() {
    ImGui::PushID(this);
    if (ImGui::CollapsingHeader(m_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
        for (auto& component: m_components) {
            const void* prop = component->getPropertyPointer();
            if (prop) {
                Inspector::drawFromProperty(component, prop);
            }
        }
    }
    ImGui::PopID();
}
#endif

void GameObject::update(float deltaTime) {
    for (auto& component : m_components) {
        component->update(*this, deltaTime);
    }
}

void GameObject::render() {
    for (auto& component : m_components) {
        component->render(*this);
    }
}

void GameObject::setContext(std::shared_ptr<RuntimeContext> ctx) {
    m_ctx = ctx;
    for (const auto& component : m_components) {
        component->setContext(ctx);
    }
}
