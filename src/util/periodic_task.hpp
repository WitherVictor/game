#pragma once

#include <chrono>
#include <condition_variable>
#include <thread>
#include <functional>

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
                std::unique_lock lock{cv_mutex_};
                cv_.wait(lock, [&st, this] {
                    return condition_ || st.stop_requested();
                });

                if (st.stop_requested())
                    return;

                cv_.wait_for(lock, duration_, [this, &st] {
                    return !condition_ || st.stop_requested();
                });

                if (st.stop_requested())
                    return;

                if (condition_)
                    std::invoke(task_);
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
private:
    std::function<void()> task_;
    std::chrono::nanoseconds duration_;

    std::jthread thread_;
    std::mutex cv_mutex_;
    std::condition_variable cv_;

    std::atomic<bool> condition_;
};