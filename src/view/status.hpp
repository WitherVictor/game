#pragma once

// STL
#include <format>

// FTXUI
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

// Project
#include "../view_model/electricity.hpp"
#include "../view_model/ore.hpp"
#include "../util/resource_type.hpp"

namespace view {

using namespace ftxui;

class status {
public:
    status() {
        auto electricity_bar = make_resource_bar<view_model::electricity>("⚡️", Color::Yellow);
        auto ore_bar = make_resource_bar<view_model::ore>("🪨", Color::White);

        component_ = Container::Vertical({
            electricity_bar,
            ore_bar
        });
    }

    static Component component() {
        static status instance;
        return instance.component_;
    }

    template <resource_t T>
    static Component make_resource_bar(std::string icon, Color gauge_color) {
        return Renderer([icon = std::move(icon), gauge_color = std::move(gauge_color)] {
            return hbox({
                text(icon),
                separator(),
                gauge(T::ratio()) | color(gauge_color),
                separator(),
                text(std::format("{}/{}",
                    T::now(),
                    T::max()
                ))
            });
        });
    }
private:
    Component component_;
};
}