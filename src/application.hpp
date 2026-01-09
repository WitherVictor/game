#pragma once

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>

#include "view/status.hpp"
#include "view/tab.hpp"
#include "screen.hpp"

class application {
public:
    static application& instance() {
        static application app;
        return app;
    }

    void run() {
        screen::instance().Loop(main_page_);
    }
private:
    application() = default;

    ftxui::Component main_page_ = ftxui::Container::Vertical({
        view::status::component(),
        ftxui::Renderer([] { return ftxui::text(""); }),
        view::tab::component()
    });
};