#pragma once

#include "engine_export.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GameObject.h>
#include <ComponentBase.h>
#include "Transform.h"

class ENGINE_EXPORT RotatorComp final : public ComponentBase<RotatorComp> {
public:
    enum class Axis : int { X = 0, Y = 1, Z = 2};

    [[field]] bool enabled = true;
    [[field]] Axis axis;
    [[field]] float speed;

    explicit RotatorComp(Axis axis = Axis::Y, float speed = 45.0f)
        : axis(axis), speed(speed) {}

    void update(GameObject& owner, float deltaTime) override {
        if (!enabled) return;

        if (Transform* tf = owner.getComponent<Transform>()) {
            switch (axis) {
                using enum Axis;
                case X: tf->rotation.x = std::fmod(tf->rotation.x + speed * deltaTime, 360.0f); break;
                case Y: tf->rotation.y = std::fmod(tf->rotation.y + speed * deltaTime, 360.0f); break;
                case Z: tf->rotation.z = std::fmod(tf->rotation.z + speed * deltaTime, 360.0f); break;
            }
        }
    }

#if defined(ENABLE_IMGUI)
    void onInspectorGUI() override;
#endif
};

inline constexpr std::array<BoolField<RotatorComp>, 1> kRotatorCompBools = {{
    { .label = "Enabled", .member = &RotatorComp::enabled }
}};

inline constexpr std::array<NumericField<RotatorComp, float>, 1> kRotatorCompFloats = {{
    { .label = "Speed", .member = &RotatorComp::speed, .step = 1.0f, .min = -360.0f, .max = 360.0f }
}};

inline const std::array<EnumField<RotatorComp>, 1> kRotatorEnumsComp = {{
    make_enum_field("Axis", &RotatorComp::axis)
}};

template <>
constexpr Property<RotatorComp> buildMetadata<RotatorComp>() {
    return Property<RotatorComp> {
        .name = "RotatorComp",
        .bools = std::span{kRotatorCompBools},
        .enums = std::span{kRotatorEnumsComp},
        .floats = std::span{kRotatorCompFloats}
    };
}
