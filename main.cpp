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

    auto power_progress_bar = Renderer([&] {
        auto state = data::power.get_state();

        return ui::resource_bar("⚡", state.current, state.maximum, Color::Yellow, 15);
    });

    auto components = Container::Vertical({
        power_progress_bar,
    });

    std::jthread generate_power_thread{[&] {
        using namespace std::chrono_literals;

        while (true) {
            data::power.try_recharge();

            std::this_thread::sleep_for(1s);
            screen.PostEvent(Event::Custom);
        }
    }};

    screen.Loop(components);
}