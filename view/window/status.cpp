// ImGui
#include "imgui.h"

// Project
#include "view/window/default_config.hpp"
#include "model/player.hpp"
#include "util/color.cpp"
#include "util/clamped_values.hpp"
#include "window_info.hpp"

namespace view::window {
    template <typename T>
    inline void make_status_bar(const char* status_name, clamped_values<T> current_status, ImVec4 color) {
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

    inline window::info status() {
        ImGui::Begin("状态", nullptr, window::default_config);

        auto window_info = GetCurrentWindowInfo();

        make_status_bar<std::size_t>("生命值", model::player.health(), Color::Red);
        make_status_bar<std::size_t>("饥饿值", model::player.hunger(), Color::Brown);
        make_status_bar<std::size_t>("口渴值", model::player.thirst(), Color::LightBlue);

        ImGui::End();

        return window_info;
    }
}