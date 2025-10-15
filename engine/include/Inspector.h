#pragma once
#include <concepts>

namespace inspector {
template <typename T>
concept EditableScalar = std::same_as<T, float> || std::same_as<T, int>;

#pragma region Field Definition
/* Checkbox()
BoolField<RotatorComp> {
    "Enabled",
    &RotatorComp::enabled,
}
*/
template <typename Owner>
struct BoolField {
    std::string_view label;
    bool Owner::* member;
};

/* DragFloat(), DragInt()
NumericField<RotatorComp, float> {
    "Speed",
    &RotatorComp::speed,
    -90.0f, 90.0f, 1.0f
}
*/
template <typename Owner, EditableScalar T>
struct NumericField {
    std::string_view label;
    T Owner::* member;
    T min, max, step;
};

/* DragFloat3()
Vec3Field<Transform> posField {
    "Position",
    &Transform::position.x,
    &Transform::position.y,
    &Transform::position.z,
    -100.0f, 100.0f, 0.05f
}
*/
template <typename Owner>
struct Vec3Field {
    std::string_view label;
    float Owner::* x;
    float Owner::* y;
    float Owner::* z;
    float min, max, step;
};
#pragma endregion

template <typename Owner>
struct Property {
    std::string_view name;

    std::span<const BoolField<Owner>>               bools   {};
    std::span<const NumericField<Owner, int>>       ints    {};
    std::span<const NumericField<Owner, float>>     floats  {};
    std::span<const Vec3Field<Owner>                vec3    {};
};

template <typename Owner>
[[nodiscard]] const Property<Owner>& reflect();
} // namespace Inspector
