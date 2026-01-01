#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>

namespace view {

using namespace ftxui;

class tab {
public:
    tab() {
        component_ = Container::Vertical({});
        component_->Add(make_tab_menu());
    }

    static Component component() {
        static tab instance;
        return instance.component_;
    }

private:
    Component make_tab_menu() {
        return Menu(&tab_menu_label_, &tab_index_, MenuOption::HorizontalAnimated());
    }

    Component component_;

    int tab_index_;
    std::vector<std::string> tab_menu_label_ = {
        "发电室", "采矿室"
    };
};

}