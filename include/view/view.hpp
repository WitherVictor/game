#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "view/component/window/status.hpp"
#include "view/component/window/tab_menu.hpp"

class view {
public:
    view() {
        page_->Add(window::make_status());
        page_->Add(window::make_tab_menu());
    }

    void run() {
        auto screen = ftxui::ScreenInteractive::TerminalOutput();
        screen.Loop(page_);
    }
private:
    ftxui::Component page_ = ftxui::Container::Vertical({});
};