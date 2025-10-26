#pragma once

#include <memory>
#include <concepts>

#include <common/runtime_context.h>
#include <editor/Inspector.h>
#include <editor/InspectorRenderer.h>

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

protected:
    std::shared_ptr<RuntimeContext> m_ctx;
    GameObject* m_owner = nullptr;

#ifdef ENABLE_IMGUI
public:
    virtual void onInspectorGUI() = 0;
    virtual std::string_view inspectorName() const = 0;

    void setProperyPointer(const void* ptr) noexcept {
        m_property = ptr;
    }

    [[nodiscard]] const void* getPropertyPointer() const noexcept {
        return m_property;
    }

private:
    const void* m_property = nullptr;
#endif
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

#ifdef ENABLE_IMGUI
    std::string_view inspectorName() const override {
        return Inspector::reflect<T>().name;
    }

    void onInspectorGUI() override {
        const Inspector::Property<T>& prop = Inspector::reflect<T>();
        Inspector::drawFromProperty<T>(*static_cast<T*>(this), prop);
    }
#endif
};
