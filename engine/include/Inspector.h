#pragma once
#include <concepts>

namespace Inspector {

template <typename T>
concept EditableScalar = std::same_as<T, int> || std::same_as<T, float>;
}
