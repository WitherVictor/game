#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include "../../view_model/electricity.hpp"

namespace view::page {

using namespace ftxui;

class generator_room {
public:
    generator_room() {
        auto solar_toggle = Toggle(&solar_label, &solar_toggle_index_);
        solar_toggle |= CatchEvent([this](Event) {
            bool status = solar_toggle_index_ == 0;
            view_model::electricity::update_solar_state(status);

            return false;
        });

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