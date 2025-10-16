#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

#include <thread>

#include "src/data.hpp"
#include "ui/element/resource_bar.hpp"

using namespace ftxui;

int main() {
    auto screen = ftxui::ScreenInteractive::TerminalOutput();

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

    screen.Loop(components);
}