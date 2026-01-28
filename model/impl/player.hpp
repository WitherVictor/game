#pragma once

// STL
#include <cstddef>

// Project
#include "util/atomic_clamped.hpp"

class player_impl {
public:
    const auto health() const {
        return health_.values();
    }

    const auto hunger() const {
        return hunger_.values();
    }

    const auto thirst() const {
        return thirst_.values();
    }

    const bool try_consume_hunger(std::size_t delta = 1) {
        return hunger_.try_minus(delta);
    }
private:
    atomic_clamped<std::size_t> health_{0, 100, 100};
    atomic_clamped<std::size_t> hunger_{0, 2500, 2000};
    atomic_clamped<std::size_t> thirst_{0, 100, 50};
};