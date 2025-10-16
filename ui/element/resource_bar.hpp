#pragma once

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>

#include <format>

#include "../../src/resource_type.hpp"

namespace ui {
    using namespace ftxui;

    inline Element resource_bar(const char* icon, resource_type::state data_state, Color progress_color, int length = 10) {
        auto current{data_state.current};
        auto maximum{data_state.maximum};

        return hbox({
            text(icon),
            separator(),
            gauge(static_cast<double>(current) / maximum) | size(ftxui::WIDTH, ftxui::EQUAL, length) | color(progress_color),
            separator(),
            text(std::format("({}/{})", current, maximum))
        });
    }
}