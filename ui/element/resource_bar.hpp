#pragma once

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>

#include <format>

namespace ui {
    using namespace ftxui;

    template <typename T>
    Element resource_bar(const char* icon, T current, T maximum, Color progress_color, int length = 10) {
        return hbox({
            text(icon),
            separator(),
            gauge(static_cast<double>(current) / maximum) | size(ftxui::WIDTH, ftxui::EQUAL, length) | color(progress_color),
            separator(),
            text(std::format("({}/{})", current, maximum))
        });
    }
}