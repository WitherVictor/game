#pragma once

// STL
#include <chrono>

// ImGui
#include "imgui.h"

// Project
#include "model/player.hpp"
#include "util/color.hpp"

namespace {
    constexpr inline static auto default_window_config = 
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;
}

namespace view {
    static void status_window() {
        ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
        ImGui::Begin("人物状态", nullptr, default_window_config);

        ImGui::Text("%s", "Health");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, Color::Red);
        ImGui::ProgressBar(model::player.health().ratio(), ImVec2{});
        ImGui::PopStyleColor();

        ImGui::End();
    }

    static void update_all(const std::chrono::milliseconds& elapsed_time) {
        
    }
};