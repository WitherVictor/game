#pragma once

// STL
#include <chrono>

// Project
#include "window/status.cpp"
#include "window/side_menu.cpp"

namespace view {
    inline void draw() {
        window::status();
        window::side_menu();
    }

    inline void update_all(const std::chrono::milliseconds& elapsed_time) {
        
    }
};