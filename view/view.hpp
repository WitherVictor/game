#pragma once

// STL
#include <chrono>

// Project
#include "window/status.cpp"

namespace view {
    inline void draw() {
        window::status();
    }

    inline void update_all(const std::chrono::milliseconds& elapsed_time) {
        
    }
};