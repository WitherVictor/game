#pragma once

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>

#include "view/status.hpp"

class application {
public:
    application() {
        main_page = ftxui::Container::Vertical({
            view::status::component()
        });
    }

    void run() {
        auto screen = ftxui::ScreenInteractive::TerminalOutput();
        screen.Loop(main_page);
    }
private:
    ftxui::Component main_page;
};