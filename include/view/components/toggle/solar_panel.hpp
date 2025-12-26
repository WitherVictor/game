#pragma once

#include <vector>
#include <string>

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component.hpp>

namespace components {
namespace toggle {
class solar_panel {
public:
    solar_panel() {
        solar_toggle_ = ftxui::Toggle(solar_labels_, &solar_index_);

        component_ = ftxui::Renderer(solar_toggle_, [&] {
            return ftxui::vbox({
                ftxui::hbox(
                    ftxui::text("🛰️ Solar panel :"), solar_toggle_->Render()
                )
            });
        });
    }

    static ftxui::Component component() {
        static solar_panel toggle;
        return toggle.component_;
    }

private:
    int solar_index_ = 1;
    std::vector<std::string> solar_labels_{"开", "关"};

    ftxui::Component component_;
    ftxui::Component solar_toggle_;
};
}
}