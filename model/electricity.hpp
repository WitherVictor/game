#pragma once

// STL
#include <chrono>
#include <cstddef>

// Project
#include "../util/atomic_clamped.hpp"
#include "../util/task.hpp"

namespace model {
namespace {
using namespace std::chrono_literals;
class electricity_impl {
public:
    electricity_impl() = default;

    electricity_impl(const electricity_impl&) = delete;
    electricity_impl& operator=(const electricity_impl&) = delete;
    electricity_impl(electricity_impl&&) = delete;
    electricity_impl& operator=(electricity_impl&&) = delete;

    void reverse_state() {
        solar_.reverse_condition();
    }

    bool try_consume(std::size_t delta = 1) {
        return value_.try_minus(delta);
    }

    double progress() const {
        return solar_.progress();
    }

    void update(const std::chrono::milliseconds& elapsed_time) {
        solar_.update(elapsed_time);
    }

    std::size_t min() const { return value_.min(); }
    std::size_t max() const { return value_.max(); }
    std::size_t now() const { return value_.now(); }
private:
    atomic_clamped<std::size_t> value_{0, 100};
    task solar_ = {[this] {
        value_.try_add();
    }, 2.5s};
};
}

inline electricity_impl electricity;
}