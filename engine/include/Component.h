#pragma once

#include <memory>

#include "common/runtime_context.h"

class GameObject;

/**
 * @brief Base component class
 *
 * Lifecycle: update -> render
 */
class Component {
public:
    virtual ~Component() = default;
    virtual void update(GameObject& owner, float deltaTime) = 0;
    virtual void render(GameObject& owner) = 0;

    void setContext(std::shared_ptr<RuntimeContext> ctx) {
        m_ctx = ctx;
    }

    void setOwner(GameObject* owner) noexcept {
        m_owner = owner;
    }

    [[nodiscard]] GameObject* owner() const noexcept {
        return m_owner;
    }

protected:
    std::shared_ptr<RuntimeContext> m_ctx;
    GameObject* m_owner = nullptr;
};
