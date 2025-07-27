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