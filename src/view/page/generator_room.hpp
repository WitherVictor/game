#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include "../../view_model/electricity.hpp"

namespace view::page {

using namespace ftxui;

class generator_room {
public:
    generator_room() {
        auto option = MenuOption::Horizontal();
        option.elements_infix = [] { return text("|") | automerge; };
        option.on_change = [this] {
            bool status = solar_toggle_index_ == 0;
            view_model::electricity::update_solar_state(status);
        };
        auto solar_toggle = Menu(&solar_label, &solar_toggle_index_, option);

        auto solar_selection = Renderer(solar_toggle, [solar_toggle] {
            return vbox({
                hbox({
                    text("🛰️太阳能发电板："), solar_toggle->Render(),
                    separator(),
                    gauge(view_model::electricity::progress()),
                    separator()
                }),
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