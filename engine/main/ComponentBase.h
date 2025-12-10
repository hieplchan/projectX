#pragma once

#include <memory>
#include <concepts>

#include <common/runtime_context.h>
#include <Metadata.h>

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

    virtual std::string_view name() const { return ""; }

    void setOwner(GameObject* owner) noexcept {
        m_owner = owner;
    }
    [[nodiscard]] GameObject* owner() const noexcept {
        return m_owner;
    }

#pragma region Lifecycle
    virtual void onDeserialized() = 0;

    // Game loop calls
    virtual void update(GameObject& owner, float deltaTime) = 0;
    virtual void render(GameObject& owner) = 0;
#pragma endregion


#ifdef ENABLE_IMGUI
    virtual void onInspectorGUI() = 0;
#endif

protected:
    WindowSettings getCtxSettings() const {
        return m_ctx->window;
    }

    GameObject* getOwner() const {
        return m_owner;
    }

private:
    std::shared_ptr<RuntimeContext> m_ctx;
    GameObject* m_owner = nullptr;
};

/**
 * @brief CRTP Helper: setup static reflection pointer automatically
 */
template<typename T>
class ComponentBase : public Component {
public:
    constexpr std::string_view name() const override {
        return reflect<T>().name;
    }

    void onDeserialized() override {
        // Default: do nothing
    }

    void update(GameObject& owner, float deltaTime) override {
        // Default: do nothing
    }

    void render(GameObject& owner) override {
        // Default: do nothing
    }
};
