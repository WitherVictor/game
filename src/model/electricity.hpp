#pragma once

// STL
#include <cstddef>

// FTXUI
#include <ftxui/component/component.hpp>

//
#include "../util/atomic_clamped.hpp"
#include "../util/periodic_task.hpp"

namespace model {
using namespace ftxui;

namespace {
using namespace std::chrono_literals;
class electricity_impl {
public:
    electricity_impl()
        : solar_{[this] { value_.try_add(); }, 2.5s} {}

    electricity_impl(const electricity_impl&) = delete;
    electricity_impl& operator=(const electricity_impl&) = delete;
    electricity_impl(electricity_impl&&) = delete;
    electricity_impl& operator=(electricity_impl&&) = delete;

    bool is_on() const {
        return solar_.is_on();
    }

    void update_solar_state(bool status) {
        solar_.set_condition(status);
    }

    bool try_consume(std::size_t delta = 1) {
        return value_.try_minus(delta);
    }

    std::size_t min() const { return value_.min(); }
    std::size_t max() const { return value_.max(); }
    std::size_t now() const { return value_.now(); }
private:
    atomic_clamped<std::size_t> value_{0, 100};
    periodic_task solar_;
};
}

inline electricity_impl electricity;
}