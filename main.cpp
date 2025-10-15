#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

#include <thread>

#include "src/data.hpp"

using namespace ftxui;

int main() {
    auto screen = ftxui::ScreenInteractive::TerminalOutput();

    auto power_progress_bar = Renderer([&] {
        auto state = data::power.get_state();

        return hbox({
            text("⚡"),
            separator(),
            gauge(static_cast<double>(state.current) / state.maximum) | size(ftxui::WIDTH, ftxui::EQUAL, 10) | color(Color::Yellow),
            separator(),
            text(std::format("({}/{})", state.current, state.maximum))
        });
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