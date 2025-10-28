#pragma once

#include <vector>
#include <memory>

#include <common/runtime_context.h>
#include <Component.h>

/**
 * @brief Game object that manages components
 *
 * Lifecycle: update -> render (applied to all components)
 */
class GameObject {
public:
    GameObject() = default;
    ~GameObject() = default;

    const std::string& name() const noexcept {
        return m_name;
    }
    void setName(const std::string& name) {
        m_name = std::move(name);
    }

    /** Update all components */
    void update(float deltaTime);

    /** Render all components */
    void render();

    /** Add a component to the game object */
    template<ComponentType T, typename... Args>
    T* addComponent(Args&&... args);

    /** Get first component of specified type */
    template<ComponentType T>
    T* getComponent();

    void setContext(std::shared_ptr<RuntimeContext> ctx);

#if defined(ENABLE_IMGUI)
    void onDrawInspector();
#endif

private:
    std::string m_name = "GameObject";

    std::vector<std::unique_ptr<Component>> m_components;
    std::shared_ptr<RuntimeContext> m_ctx;
};

#include "GameObject.inl"
