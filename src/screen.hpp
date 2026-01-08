#pragma once

// FTXUI
#include "ftxui/component/screen_interactive.hpp"
#include <thread>

using namespace std::chrono_literals;

class screen {
public:
    static ftxui::ScreenInteractive& instance() {
        static screen instance;
        return instance.screen_;
    }

    static void request_frame() {
        instance().RequestAnimationFrame();
    }
private:
    screen() {
        thread_ = std::jthread([] {
            while (true) {
                std::this_thread::sleep_for(16ms);
                screen::request_frame();
            }
        });
    }

    std::jthread thread_;
    ftxui::ScreenInteractive screen_ = ftxui::ScreenInteractive::TerminalOutput();
};