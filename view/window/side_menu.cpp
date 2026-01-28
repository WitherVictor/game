#include "imgui.h"

#include "default_config.hpp"
#include "current_room.hpp"
#include "window_info.hpp"

namespace view::window {
    inline window::info side_menu() {
        // 创建窗口
        ImGui::Begin("空间站", nullptr, window::default_config);
        auto window_info = GetCurrentWindowInfo();

        ImGui::Text("%s", "工程部");
        ImGui::Indent();

        if (ImGui::Selectable("机械发电室")) {
            current_room = room_type::engi_mechgen;
        }

        if (ImGui::Selectable("储能电站")) {
            current_room = room_type::engi_powerstore;
        }

        ImGui::End();

        return window_info;
    }
}