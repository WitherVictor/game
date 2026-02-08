#ifndef UTIL_TASK_TIMER_TASK_HPP
#define UTIL_TASK_TIMER_TASK_HPP

#include <chrono>
#include <shared_mutex>
#include <functional>
#include <mutex>

#include "task.hpp"

using namespace std::chrono_literals;

// 定时执行的任务，只有开启和关闭状态
class timer_task final : public task {
public:
    template <typename Rep, typename Duration>
    timer_task(std::function<void()> func, std::chrono::duration<Rep, Duration> delay, bool condition = false)
        : task{std::move(func)}, duration{std::chrono::duration_cast<std::chrono::milliseconds>(delay)}, condition{condition} {}

    bool update(std::chrono::milliseconds elapsed_time) override {
        std::unique_lock lock{mutex};

        if (!condition)
            return true;

        current += elapsed_time;

        if (current >= duration) {
            current -= duration;
            lock.unlock();
            std::invoke(task);
        }

        return true;
    }

    long double progress() const override {
        if (duration == 0ms)
            return 1.0;

        return static_cast<long double>(current.count()) / duration.count();
    }

    void start() {
        std::unique_lock lock{mutex};
        condition = true;
    }

    void stop() {
        std::unique_lock lock{mutex};
        current = 0ms;
        condition = false;
    }
private:
    mutable std::shared_mutex mutex;
    std::function<void()> task;
    std::chrono::milliseconds current = 0ms;
    std::chrono::milliseconds duration;
    bool condition = false;
};

#endif // End of macro UTIL_TASK_TIMER_TASK_HPP