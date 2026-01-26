#pragma once

// STL
#include <cstddef>

// Project
#include "util/atomic_clamped.hpp"

namespace model {
class player_impl {
public:
    const auto health() const {
        return health_.values();
    }
private:
    atomic_clamped<std::size_t> health_{0, 100, 100};
    atomic_clamped<std::size_t> hunger_{0, 100};
    atomic_clamped<std::size_t> thirst_{0, 100};
    atomic_clamped<std::size_t> stamina_{0, 100};
};

inline player_impl player;
}