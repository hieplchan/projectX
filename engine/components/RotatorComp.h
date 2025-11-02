#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine_export.h"

#include <GameObject.h>
#include <Component.h>
#include "Transform.h"

class ENGINE_EXPORT RotatorComp : public ComponentBase<RotatorComp> {
public:
    enum class Axis : int { X = 0, Y = 1, Z = 2};

    bool enabled = true;
    Axis axis;
    float speed;

    explicit RotatorComp(Axis axis = Axis::Y, float speed = 45.0f)
        : axis(axis), speed(speed) {}

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

    void render(GameObject& owner) override {}

#if defined(ENABLE_IMGUI)
    void onInspectorGUI() override;
#endif
};

inline constexpr BoolField<RotatorComp> kRotatorCompBools[] = {
    { .label = "Enabled", .member = &RotatorComp::enabled }
};

inline constexpr NumericField<RotatorComp, float> kRotatorCompFloats[] = {
    { .label = "Speed", .member = &RotatorComp::speed, .step = 1.0f, .min = 0.0f, .max = 360.0f }
};

template <>
inline constexpr Property<RotatorComp> buildMetadata<RotatorComp>() {
    return Property<RotatorComp> {
        .name = "RotatorComp",
        .bools = std::span{kRotatorCompBools},
        .floats = std::span{kRotatorCompFloats}
    };
}
