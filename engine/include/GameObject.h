#pragma once

#include <vector>
#include <memory>

#include "Component.h"

/**
 * @brief Game object that manages components
 *
 * Lifecycle: update -> render (applied to all components)
 */
class GameObject {
public:
    GameObject() = default;
    ~GameObject() = default;

    /** Update all components */
    void update(float deltaTime);

    /** Render all components */
    void render();

    /** Add a component to the game object */
    template<typename T, typename... Args>
    T* addComponent(Args&&... args);

    /** Get first component of specified type */
    template<typename T>
    T* getComponent();

private:
    std::vector<std::unique_ptr<Component>> m_components;
};

#include "GameObject.inl"
