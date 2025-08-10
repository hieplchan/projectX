#include "GameObject.h"

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
