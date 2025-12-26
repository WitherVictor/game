#pragma once

#include <vector>
#include <string>

#include <ftxui/component/component_options.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "solar_panel_toggle.hpp"

namespace page {
inline ftxui::Component make_tab_menu() {
    static int tab_index = 0;
    static std::vector<std::string> tab_label = {"发电室", "测试"};

    return ftxui::Container::Vertical({
        ftxui::Menu(&tab_label, &tab_index, ftxui::MenuOption::HorizontalAnimated()),
        ftxui::Container::Tab({
            components::solar_panel_toggle::component()
        }, &tab_index)
    });
}
}