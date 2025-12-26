#pragma once

#include <format>
#include <ftxui/component/component_base.hpp>
#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>


namespace component {
template <typename T>
inline ftxui::Component make_status_bar(std::string icon) {
    constexpr int default_width = 15;

    return ftxui::Renderer([icon = std::move(icon)] {
        return ftxui::hbox({
            ftxui::text(icon),
            ftxui::separator(),
            ftxui::gauge(T::ratio()) | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, default_width),
            ftxui::separator(),
            ftxui::text(std::format("{}/{}",
                T::current(),
                T::maximum()
            ))
        }) | ftxui::border;
    });
}
}