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

    template <typename T>
    inline void make_status_bar(const char* status_name, typename clamped<T>::info current_status, ImVec4 color) {
        // 状态名
        ImGui::Text("%s", status_name);
        ImGui::SameLine();

        // 设置进度条颜色
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
        ImGui::ProgressBar(current_status.ratio, ImVec2{});
        ImGui::PopStyleColor();
        ImGui::SameLine();

        // 显示当前值与最大值
        ImGui::Text("[%zu/%zu]", current_status.now, current_status.max);
    }

    inline void status() {
        ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
        ImGui::Begin("人物状态", nullptr, default_window_config);

        make_status_bar<std::size_t>("生命值", model::player.health(), Color::Red);
        make_status_bar<std::size_t>("饥饿值", model::player.hunger(), Color::Brown);
        make_status_bar<std::size_t>("口渴值", model::player.thirst(), Color::LightBlue);

        ImGui::End();
    }
}