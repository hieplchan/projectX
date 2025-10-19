#pragma once
#include <Transform.h>
#include <Component.h>
#include <Inspector.h>

class RotatorComp : public Component {
public:
    enum class Axis : int { X = 0, Y = 1, Z = 2};

    bool enabled = true;
    Axis axis;
    float speed;

    explicit RotatorComp(Axis axis = Axis::Y, float speed = 45.0f)
        : axis(axis), speed(speed) {}

    void render(GameObject& owner) override {}

    void update(GameObject& owner, float deltaTime) override {
        if (!enabled) return;

        if (Transform* tf = owner.getComponent<Transform>()) {
            switch (axis) {
                case Axis::X: tf->rotation.x += speed * deltaTime; break;
                case Axis::Y: tf->rotation.y += speed * deltaTime; break;
                case Axis::Z: tf->rotation.z += speed * deltaTime; break;
            }
        }
    }
};

#ifdef ENABLE_IMGUI
namespace Inspector {
inline constexpr BoolField<RotatorComp> kBools[] = {
    { .label = "Enabled", .member = &RotatorComp::enabled }
};

template <>
constexpr Property<RotatorComp> buildMetadata<RotatorComp>() {
    return Property<RotatorComp> {
        .name = "RotatorComp",
        .bools = std::span{kBools}
    };
}
} // namespace Inspector
#endif
