#pragma once

#include <format>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "../view_model/electricity.hpp"
#include "../util/resource_type.hpp"

namespace view {

using namespace ftxui;

class status {
public:
    status() {
        auto electricity_bar = make_resource_bar<view_model::electricity>("⚡️");

        component_ = Container::Vertical({});
        component_->Add(electricity_bar);
    }

    static Component component() {
        static status instance;
        return instance.component_;
    }

    template <resource_t T>
    static Component make_resource_bar(std::string icon) {
        return Renderer([icon = std::move(icon)] {
            return hbox({
                text(icon),
                separator(),
                gauge(T::ratio()),
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