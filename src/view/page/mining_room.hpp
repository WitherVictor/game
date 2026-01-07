#pragma once

// FTXUI
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

// Project
#include "../../view_model/ore.hpp"

namespace view::page {
using namespace ftxui;

class mining_room {
public:
    mining_room() {
        auto iron_toggle = Toggle(&iron_toggle_label_, &iron_toggle_index_);
        iron_toggle |= CatchEvent([this] (Event) {
            bool status = iron_toggle_index_ == 0;
            view_model::ore::set_miner_state(status);

            return false;
        });

        auto iron_selection = Renderer(iron_toggle, [iron_toggle] {
            return hbox({
                text("挖铁："),
                iron_toggle->Render()
            });
        });

        component_ = Container::Vertical({
            iron_selection
        });
    }

    // 返回渲染的页面组件
    static Component component() {
        static mining_room instance;
        return instance.component_;
    }
private:
    int iron_toggle_index_ = 1;
    const std::vector<std::string> iron_toggle_label_ = {
        "开", "关"
    };

    Component component_;
};

} // End of namespace view::page