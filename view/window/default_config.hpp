#pragma once

#include "imgui.h"

namespace view::window {
    inline constexpr ImGuiWindowFlags default_config = 
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_AlwaysAutoResize;
}