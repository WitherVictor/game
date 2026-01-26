#pragma once

#include <concepts>

// 算术类型概念
template <typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

// 保存 clamped 所有数据的类型
template <arithmetic T>
struct clamped_values {
    T min;
    T max;

    T now;

    long double ratio;
};