#pragma once

#include <memory>
#include <concepts>

#include <common/runtime_context.h>
#include <editor/InspectorMetadata.h>
#include <editor/Inspector.h>

class GameObject;

template<typename T>
concept ComponentType = std::derived_from<T, class Component>;

/**
 * @brief Base component class
 *
 * Lifecycle: update -> render
 */
class Component {
public:
    virtual ~Component() = default;

    void setContext(std::shared_ptr<RuntimeContext> ctx) noexcept {
        m_ctx = ctx;
    }

    virtual void update(GameObject& owner, float deltaTime) = 0;
    virtual void render(GameObject& owner) = 0;

    void setOwner(GameObject* owner) noexcept {
        m_owner = owner;
    }
    [[nodiscard]] GameObject* owner() const noexcept {
        return m_owner;
    }

protected:
    std::shared_ptr<RuntimeContext> m_ctx;
    GameObject* m_owner = nullptr;

#ifdef ENABLE_IMGUI
    // vtable entry, let editor TU override drawing code
    virtual void onDrawInspector() const = 0;
#endif
};

/**
 * @brief CRTP Helper: setup static reflection pointer automatically
 */
template<typename T>
class ComponentBase : public Component {
#ifdef ENABLE_IMGUI
public:
    // declare override, no impl
    void onDrawInspector() const override;
#endif
};

// #ifdef ENABLE_IMGUI
// #define REGISTER_COMPONENT_FOR_EDITOR(Type)           \
//     namespace {                                       \
//         /* Force instantiation of ComponentBase<Type> */ \
//         template class ComponentBase<Type>;           \
//         /* Force instantiation of Inspector::drawInspector<Type> */ \
//         template void Inspector::drawInspector<Type>(Type*); \
//     }

// #else

// #define REGISTER_COMPONENT_FOR_EDITOR(Type) do {} while(0)

// #endif
