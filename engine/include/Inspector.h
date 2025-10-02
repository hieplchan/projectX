#pragma once
#include <concepts>

namespace inspector {

/*
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
} // namespace Inspector
