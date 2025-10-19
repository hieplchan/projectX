#pragma once
#include <concepts>

namespace Inspector {

template <typename Object>
struct Property {
    std::string_view name;

    std::span<const BoolField<Object>>               bools   {};
    std::span<const NumericField<Object, int>>       ints    {};
    std::span<const NumericField<Object, float>>     floats  {};
    std::span<const Vec3Field<Object>                vec3    {};
};

// user define their own type specification in "Component code"
template <typename Object>
constexpr Property<Object> buildMetadata();

// "Engine code" call this to get type info, just wrapper about buildMetadata
template <typename Object>
[[nodiscard]] inline const Property<Object>& reflect() {
    static constinit Property<Object> prop = buildMetadata<Object>();
    return prop;
}

#pragma region Field Definition
template <typename T>
concept EditableScalar = std::same_as<T, float> || std::same_as<T, int>;

/* Checkbox()
BoolField<RotatorComp> {
    "Enabled",
    &RotatorComp::enabled,
}
*/
template <typename Object>
struct BoolField {
    std::string_view label;
    bool Object::* member;
};

/* DragFloat(), DragInt()
NumericField<RotatorComp, float> {
    "Speed",
    &RotatorComp::speed,
    -90.0f, 90.0f, 1.0f
}
*/
template <typename Object, EditableScalar T>
struct NumericField {
    std::string_view label;
    T Object::* member;
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
template <typename Object>
struct Vec3Field {
    std::string_view label;
    float Object::* x;
    float Object::* y;
    float Object::* z;
    float min, max, step;
};
#pragma endregion
} // namespace Inspector
