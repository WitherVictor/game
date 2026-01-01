#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace view::page {

using namespace ftxui;

class generator_room {
public:
    generator_room() {
        auto solar_toggle = Toggle(&solar_label, &solar_toggle_index_);
        auto solar_selection = Renderer(solar_toggle, [solar_toggle] {
            return vbox({
                hbox({ text("🛰️太阳能发电板："), solar_toggle->Render()})
            });
        });

        component_ = Container::Vertical({
            solar_selection
        });
    }

    static Component component() {
        static generator_room instance;
        return instance.component_;
    }
private:
    int solar_toggle_index_ = 1;
    std::vector<std::string> solar_label = {
        "开", "关"
    };

    Component component_;
};

}