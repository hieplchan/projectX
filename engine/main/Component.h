#pragma once

#include <memory>
#include <concepts>

#include <common/runtime_context.h>
#include <editor/InspectorMetadata.h>

template<typename T>
concept ComponentType = std::derived_from<T, class Component>;

class GameObject;

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

#ifdef ENABLE_IMGUI
    void setProperyPointer(const void* ptr) noexcept {
        m_property = ptr;
    }
    [[nodiscard]] const void* getPropertyPointer() const noexcept {
        return m_property;
    }
    virtual void onInspectorGUI() {}
    virtual std::string_view inspectorName() const {
        return typeid(*this).name();
    }
private:
    const void* m_property = nullptr;
#endif

protected:
    std::shared_ptr<RuntimeContext> m_ctx;
    GameObject* m_owner = nullptr;
};

/**
 * @brief CRTP Helper: setup static reflection pointer automatically
 */
template<typename T>
class ComponentBase : public Component {
public:
    ComponentBase() {
#ifdef ENABLE_IMGUI
        setProperyPointer(&Inspector::reflect<T>());
#endif
    }
};
