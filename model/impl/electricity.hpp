#pragma once

// STL
#include <cstddef>

// Project
#include "util/atomic_clamped.hpp"

class electricity_impl {
public:
    electricity_impl() = default;

    electricity_impl(const electricity_impl&) = delete;
    electricity_impl& operator=(const electricity_impl&) = delete;
    electricity_impl(electricity_impl&&) = delete;
    electricity_impl& operator=(electricity_impl&&) = delete;

    bool try_restore(std::size_t delta = 1) {
        return value_.try_add(delta);
    }

    bool try_consume(std::size_t delta = 1) {
        return value_.try_minus(delta);
    }

    std::size_t min() const { return value_.min(); }
    std::size_t max() const { return value_.max(); }
    std::size_t now() const { return value_.now(); }
private:
    atomic_clamped<std::size_t> value_{0, 100};
};