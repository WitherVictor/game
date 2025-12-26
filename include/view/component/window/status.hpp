#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "view_model/view_model.hpp"
#include "view/component/status_bar.hpp"

namespace window {
inline ftxui::Component make_status() {
    return ftxui::Renderer([] {
        return ftxui::hbox({
            ftxui::vbox(
                component::make_status_bar<view_model::electricity>("⚡️")->Render()
            )
        });
    });
}
}