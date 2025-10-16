#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

#include <thread>

#include "src/data.hpp"
#include "ui/element/resource_bar.hpp"

using namespace ftxui;
using namespace std::chrono_literals;

int main() {
    auto screen = ftxui::ScreenInteractive::FitComponent();

    auto status_progress_bar = Renderer([&] {
        auto power_state = data::power.get_state();
        auto stamina_state = data::stamina.get_state();

        return vbox({
            ui::resource_bar("🔋", power_state, Color::Yellow, 15),
            ui::resource_bar("⚡", stamina_state, Color::White, 15)
        }) | border;
    });

    auto generate_power_button = Button("Generate Power", [] {
        if (data::stamina.try_comsume(10)) {
            data::power.try_recharge();
        }
    }) | flex;

    auto components = Container::Vertical({
        status_progress_bar,
        generate_power_button
    });

    std::jthread generate_power_thread{[&] {
        using namespace std::chrono_literals;

        while (true) {
            data::stamina.try_recharge();

            std::this_thread::sleep_for(1s);
            screen.PostEvent(Event::Custom);
        }
    }};

    int tab_selector{};
    std::vector<std::string> tab_titles{"Electrical Room", "Not Available"};

    auto option = MenuOption::HorizontalAnimated();
    option.underline.SetAnimationDuration(250ms);

    components->Add(Menu(&tab_titles, &tab_selector, option));

    screen.Loop(components);
}