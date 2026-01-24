#pragma once

// STL
#include <chrono>

// ImGui
#include "imgui.h"

// Project
#include "model/electricity.hpp"

namespace view {
    static void status_window() {
        ImGui::Begin("资源");

        ImGui::ProgressBar(model::electricity.progress());

        ImGui::End();
    }

    static void update_all(const std::chrono::milliseconds& elapsed_time) {
        model::electricity.update(elapsed_time);
    }
};