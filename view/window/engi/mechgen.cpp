// ImGui
#include "imgui.h"

// Project
#include "model/model.hpp"

#include "view/window/default_config.hpp"
#include "view/window/window_info.hpp"

#include "util/task.hpp"
#include "util/color.cpp"

namespace view::window::engi {

class mechgen {
public:
    mechgen() {
        model::instance().register_task(task_ptr_);
    }

    static window::info draw() {
        static mechgen instance;
        return instance.draw_impl();
    }

    window::info draw_impl() {
        ImGui::Begin("机械发电室", nullptr, window::default_config);
        auto window_info = GetCurrentWindowInfo();

        if (ImGui::Checkbox("人力发电机", &is_checked_)) {
            task_ptr_->reverse_condition();
        }

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, Color::Yellow);
        ImGui::ProgressBar(task_ptr_->progress(), ImVec2{});
        ImGui::PopStyleColor();
        
        ImGui::End();

        return window_info;
    }
private:
    bool is_checked_ = false;
    std::shared_ptr<task> task_ptr_ = std::make_shared<task>([] {
            if (model::player().try_consume_hunger()) {
                model::electricity().try_restore();
            }
    }, 1s);
};

}