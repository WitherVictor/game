#pragma once

#include "imgui.h"

namespace view::window {
    struct info {
        ImVec2 pos;
        ImVec2 size;
    };

    inline info GetCurrentWindowInfo() {
        return {
            .pos = ImGui::GetWindowPos(),
            .size = ImGui::GetWindowSize(),
        };
    }
}