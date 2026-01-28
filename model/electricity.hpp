#pragma once

// STL
#include <cstddef>

// Project
#include "util/atomic_clamped.hpp"

struct electricity {
    atomic_clamped<std::size_t> power{0, 100};
};