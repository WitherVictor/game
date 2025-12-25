#pragma once

#include <format>
#include <ftxui/component/component_base.hpp>
#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>


namespace component {
    template <typename T>
    inline ftxui::Component make_status_bar(std::string icon) {
        return ftxui::Renderer([icon = std::move(icon)] {
            return ftxui::hbox({
                ftxui::text(icon),
                ftxui::separator(),
                ftxui::gauge(T::ratio()),
                ftxui::separator(),
                ftxui::text(std::format("{}/{}",
                    T::current(),
                    T::maximum()
                ))
            });
        });
    }
}