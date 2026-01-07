#pragma once

// STL
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <stop_token>
#include <thread>

// Project
#include "../util/atomic_clamped.hpp"
#include "../util/periodic_task.hpp"
#include "electricity.hpp"

namespace model {
namespace {
using namespace std::chrono_literals;

class ore_impl {
public:
    ore_impl()
        : electricity_to_ore_{[this] {
            if (model::electricity.try_consume(10))
                try_produce();
        }, 10s}
    {}

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
        electricity_to_ore_.set_condition(state);
    }

    bool try_produce(std::size_t delta = 1) {
        return value_.try_add(delta);
    }
private:
    atomic_clamped<std::size_t> value_{0, 100};
    periodic_task electricity_to_ore_;
};
} // End of anomynous namespace

inline ore_impl ore;
} // End of namespace model