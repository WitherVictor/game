#pragma once

// FTXUI
#include "ftxui/component/screen_interactive.hpp"
#include "model/electricity.hpp"
#include "model/ore.hpp"
#include <chrono>
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
        thread_ = std::jthread([this] {
            while (true) {
                std::this_thread::sleep_for(16ms);
                update_all();
                screen::request_frame();
            }
        });
    }

    static void update_all() {
        static auto before = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - before);
        before = now;

        model::electricity.update(elapsed);
        model::ore.update(elapsed);
        
        auto process = std::chrono::steady_clock::now() - now;
        if (process > 0ms)
            std::this_thread::sleep_for(process);
    }

    std::jthread thread_;
    ftxui::ScreenInteractive screen_ = ftxui::ScreenInteractive::TerminalOutput();
};