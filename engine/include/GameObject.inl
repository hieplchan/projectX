#pragma once

template<ComponentType T, typename... Args>
T* GameObject::addComponent(Args&&... args) {
    auto component = std::make_unique<T>(std::forward<Args>(args)...);
    T* rawPtr = component.get();
    rawPtr->setOwner(this);
    m_components.push_back(std::move(component));
    return rawPtr;
}

template <ComponentType T>
T* GameObject::getComponent() {
    for (auto& comp : m_components) {
        if (T* ptr = dynamic_cast<T*>(comp.get())) {
            return ptr;
        }
    }
    return nullptr;
}
