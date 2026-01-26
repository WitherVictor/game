#include "imgui.h"

#include "default_config.hpp"

namespace view::window {
    enum class room_type {
        none,
        engi_mechgen,
        engi_powerstore
    };

    inline static room_type current_room = room_type::none;

    inline void side_menu() {
        // 创建窗口
        ImGui::Begin("空间站", nullptr, window::default_config);

        ImGui::Text("%s", "工程部");
        ImGui::Indent();

        if (ImGui::Selectable("机械发电室")) {
            current_room = room_type::engi_mechgen;
        }

        if (ImGui::Selectable("储能电站")) {
            current_room = room_type::engi_powerstore;
        }

        ImGui::End();
    }
}