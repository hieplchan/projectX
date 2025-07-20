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

template<typename T, typename... Args>
T* GameObject::addComponent(Args&&... args) {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
    auto component = std::make_unique<T>(std::forward<Args>(args)...);
    T* rawPtr = component.get();
    m_components.push_back(std::move(component));
    return rawPtr;
}

template <typename T>
T* GameObject::getComponent()
{
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
    for (auto& comp : m_components) {
        if (T* ptr = dynamic_cast<T*>(comp.get())) {
            return ptr;
        }
    }
    return nullptr;
}
