#pragma once

#include "imgui.h"

namespace view::window {
    inline constexpr ImGuiWindowFlags default_config = 
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    // 窗口大小位置信息类型
    struct info {
        ImVec2 pos;
        ImVec2 size;
    };

    inline info GetCurrentWindowInfo() {
        return {
            .pos = ImGui::GetWindowPos(),
            .size = ImGui::GetWindowSize()
        };
    }
}