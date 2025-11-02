#pragma once
#include <concepts>
#include <string_view>
#include <span>
#include <glm/glm.hpp>
#include <magic_enum/magic_enum.hpp>

// Owner: our engine object, mimic Unreal UObject
#pragma region Field Definition

template <typename T>
concept EditableScalar = std::same_as<T, float> || std::same_as<T, int>;

template <typename T>
concept EnumInt = std::is_enum_v<T> && std::is_same_v<std::underlying_type_t<T>, int>;

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

template <typename Owner>
struct ColorField {
    std::string_view label;
    glm::vec4 Owner::* color;
};

template <typename Owner>
struct EnumField {
    std::string_view label;
    int Owner::* member;
    std::span<const std::string_view> names;
    std::span<const int> values;
};
template <EnumInt EnumT>
consteval auto make_enum_values() {
    constexpr auto values = magic_enum::enum_values<EnumT>();
    std::array<int, values.size()> out{};
    for (std::size_t i = 0; i < out.size(); i++) {
        out[i] = static_cast<int>(values[i]);
    }
    return out;
}
template <typename Owner, EnumInt EnumT>
inline const EnumField<Owner>& make_enum_field(std::string_view label, EnumT Owner::* member) {
    static constexpr auto names = magic_enum::enum_names<EnumT>();
    static constexpr auto values = make_enum_values<EnumT>();

    static const EnumField<Owner> field = {
        .label = label,
        .member = reinterpret_cast<int Owner::*>(member),
        .names = std::span{names},
        .values = std::span{values}
    };

    return field;
}

#pragma endregion

template <typename Owner>
struct Property {
    std::string_view name;
    std::span<const BoolField<Owner>>               bools   {};
    std::span<const EnumField<Owner>>               enums   {};
    std::span<const NumericField<Owner, int>>       ints    {};
    std::span<const NumericField<Owner, float>>     floats  {};
    std::span<const Vec3Field<Owner>>               vec3s   {};
    std::span<const ColorField<Owner>>              colors  {};
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
