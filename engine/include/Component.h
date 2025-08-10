#pragma once

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
};
