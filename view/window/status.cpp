// ImGui
#include "imgui.h"

#include "model/player.hpp"
#include "util/color.cpp"

namespace view::window {
    constexpr inline static auto default_window_config = 
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    inline void status() {
        ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
        ImGui::Begin("人物状态", nullptr, default_window_config);

        const auto player_health = model::player.health();
        ImGui::Text("%s", "Health");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, Color::Red);
        ImGui::ProgressBar(player_health.ratio, ImVec2{});
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::Text("[%zu/%zu]", player_health.now, player_health.max);

        ImGui::End();
    }
}