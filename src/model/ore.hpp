#pragma once

// STL
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <stop_token>
#include <thread>

// Project
#include "../util/atomic_clamped.hpp"
#include "../util/task.hpp"
#include "electricity.hpp"

namespace model {
namespace {
using namespace std::chrono_literals;

class ore_impl {
public:
    ore_impl() = default;

    // 独占资源禁止复制和移动
    ore_impl(const ore_impl&) = delete;
    ore_impl& operator=(const ore_impl&) = delete;
    ore_impl(ore_impl&&) = delete;
    ore_impl& operator=(ore_impl&&) = delete;

    // 元素访问
    std::size_t min() const { return value_.min(); }
    std::size_t max() const { return value_.max(); }
    std::size_t now() const { return value_.now(); }

    void set_mining_state(bool state) {
        state ? electricity_to_ore_.start() : electricity_to_ore_.stop();
    }

    bool try_produce(std::size_t delta = 1) {
        return value_.try_add(delta);
    }

    void update(std::chrono::milliseconds elapsed_time) {
        electricity_to_ore_.update(elapsed_time);
    }

    double progress() const {
        return electricity_to_ore_.progress();
    }
private:
    atomic_clamped<std::size_t> value_{0, 100};
    task electricity_to_ore_ = {[this, &electricity = model::electricity] {
        if (electricity.try_consume(10))
            value_.try_add();
    }, 10s};
};
} // End of anomynous namespace

inline ore_impl ore;
} // End of namespace model