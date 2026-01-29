#ifndef MODEL_MODEL_HPP
#define MODEL_MODEL_HPP

#include <chrono>
#include <stop_token>
#include <thread>

#include "electricity.hpp"
#include "player.hpp"
#include "task_manager.hpp"

using namespace std::chrono_literals;

class model {
public:
    inline const static auto frame_rate = 16ms;

    player& get_player() {
        return player_;
    }

    electricity& get_electricity() {
        return electricity_;
    }
private:
    electricity electricity_;

    player player_;

    std::jthread thread_{[](std::stop_token st) {
        auto next_frame = std::chrono::steady_clock::now();

        while (!st.stop_requested()) {
            task_manager::instance().update_all(frame_rate);

            // 推进一帧并等待直到下一帧
            next_frame += frame_rate;
            std::this_thread::sleep_until(next_frame);
        }
    }};
};

#endif // End of MODEL_MODEL_HPP