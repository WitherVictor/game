#pragma once

// STL
#include <chrono>
#include <condition_variable>
#include <thread>
#include <functional>

// Project
#include "../screen.hpp"

using namespace std::chrono_literals;

class periodic_task {
public:
    template <typename T, typename Rep, typename Period>
    periodic_task(T&& callable, std::chrono::duration<Rep, Period> duration)
        : task_{[callable = std::move(callable)] { std::invoke(callable); }}, 
          duration_{std::chrono::duration_cast<std::chrono::nanoseconds>(duration)},
          condition_{false}
    {
        thread_ = std::jthread([this](std::stop_token st) {
            while (!st.stop_requested()) {
                {
                    //  仅用于通知线程
                    std::unique_lock lock{cv_mutex_};
                    cv_.wait(lock, [&st, this] {
                        return condition_ || st.stop_requested();
                    });
                }

                //  如果要求线程停止，则立即返回
                if (st.stop_requested())
                    return;

                //  以 40FPS 的速率进行资源更新
                while (progress_ < duration_ && condition_) {
                    std::this_thread::sleep_for(25ms);
                    progress_ += 25ms;
                }

                //  如果时长和条件已满足，则运行任务
                if (condition_)
                    std::invoke(task_);

                //  重置进度条进度
                progress_ = 0ns;
            }
        });
    }

    bool is_on() const {
        return condition_;
    }

    void set_condition(bool new_condition) {
        condition_ = new_condition;
        cv_.notify_one();
    }

    template <typename Rep, typename Period>
    void set_duration(std::chrono::duration<Rep, Period> new_duration) {
        duration_ = new_duration;
    }

    double progress_ratio() const {
        auto progress = static_cast<double>(progress_.count());
        auto duration = static_cast<double>(duration_.count());

        return progress / duration;
    }
private:
    std::function<void()> task_;
    std::chrono::nanoseconds progress_ = 0ns;
    std::chrono::nanoseconds duration_;

    std::jthread thread_;
    std::mutex cv_mutex_;
    std::condition_variable cv_;

    std::atomic<bool> condition_;
};