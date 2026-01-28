#pragma once

// STL
#include <cstddef>

// Project
#include "util/atomic_clamped.hpp"

struct player {
    atomic_clamped<std::size_t> health{0, 100, 100};
    atomic_clamped<std::size_t> hunger{0, 2500, 2000};
    atomic_clamped<std::size_t> thirst{0, 100, 50};
};