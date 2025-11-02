#pragma once
#include <concepts>
#include <string_view>
#include <span>
#include <glm/glm.hpp>

// Owner: our engine object, mimic Unreal UObject
#pragma region Field Definition
template <typename T>
concept EditableScalar = std::same_as<T, float> || std::same_as<T, int>;

template <typename Owner>
struct BoolField {
    std::string_view label;
    bool Owner::* member;
};

template <typename Owner, EditableScalar T>
struct NumericField {
    std::string_view label;
    T Owner::* member;
    T step, min, max;
};

template <typename Owner>
struct Vec3Field {
    std::string_view label;
    glm::vec3 Owner::* vec3;
    float step, min, max;
};
#pragma endregion

template <typename Owner>
struct Property {
    std::string_view name;
    std::span<const BoolField<Owner>>               bools   {};
    std::span<const NumericField<Owner, int>>       ints    {};
    std::span<const NumericField<Owner, float>>     floats  {};
    std::span<const Vec3Field<Owner>>               vec3    {};
};

// user define their own type specification in "Component code"
template <typename Owner>
constexpr Property<Owner> buildMetadata();

// "Engine code" call this to get type info, just wrapper about buildMetadata
template <typename Owner>
[[nodiscard]] inline const Property<Owner>& reflect() {
    static constinit Property<Owner> prop = buildMetadata<Owner>();
    return prop;
}
