#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "../../view_model/view_model.hpp"
#include "status_bar.hpp"

namespace page {
inline ftxui::Component make_status_page() {
    return ftxui::Renderer([] {
        return ftxui::hbox({
            ftxui::vbox(
                component::make_status_bar<view_model::electricity>("⚡️")->Render()
            )
        });
    });
}
}