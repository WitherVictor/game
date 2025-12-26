#pragma once

#include <concepts>

template <typename T>
concept resource_t = requires {
    { T::now() } -> std::same_as<std::size_t>;
    { T::max() } -> std::same_as<std::size_t>;
    { T::ratio() } -> std::same_as<double>;
};