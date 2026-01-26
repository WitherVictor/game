#pragma once

// STL
#include <chrono>

// Project
#include "window/status.cpp"
#include "window/side_menu.cpp"

namespace view {
    inline void draw() {
        auto status_window_info = window::status();
        window::side_menu(status_window_info);
    }

    inline void update_all(const std::chrono::milliseconds& elapsed_time) {
        
    }
};