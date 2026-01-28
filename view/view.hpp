#pragma once

// STL
#include <chrono>

// Project
#include "imgui.h"
#include "window/status.cpp"
#include "window/side_menu.cpp"
#include "window/main_window.cpp"

namespace view {
    inline void draw() {
        constexpr ImVec2 default_window_pos = {0, 0};
        ImGui::SetNextWindowPos(default_window_pos);
        auto status_window_info = window::status();

        ImGui::SetNextWindowPos(ImVec2{
            status_window_info.pos.x,
            status_window_info.pos.y + status_window_info.size.y
        });
        auto menu_window_info = window::side_menu();

        ImGui::SetNextWindowPos(ImVec2{
            menu_window_info.pos.x + menu_window_info.size.x,
            menu_window_info.pos.y
        });
        window::main_window();
    }

    inline void update_all(const std::chrono::milliseconds& elapsed_time) {
        model::instance().update_all(elapsed_time);
    }
};