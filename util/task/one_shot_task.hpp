#ifndef UTIL_TASK_ONE_SHOT_TASK_HPP
#define UTIL_TASK_ONE_SHOT_TASK_HPP

#include <chrono>
#include <shared_mutex>
#include <functional>
#include <mutex>

#include "task.hpp"

using namespace std::chrono_literals;

// 一次性的执行任务
class one_shot_task final : public task {
public:
    template <typename Rep, typename Duration>
    one_shot_task(std::function<void()> func, std::chrono::duration<Rep, Duration> delay)
        : task{std::move(func)}, duration{std::chrono::duration_cast<std::chrono::milliseconds>(delay)} {}

    bool update(std::chrono::milliseconds elapsed_time) override {
        std::unique_lock lock{mutex};

        current += elapsed_time;

        if (current >= duration) {
            lock.unlock();
            std::invoke(task);

            return false;
        }

        return true;
    }

    long double progress() const override {
        if (duration == 0ms)
            return 1.0;

        return static_cast<long double>(current.count()) / duration.count();
    }
private:
    mutable std::shared_mutex mutex;
    std::function<void()> task;
    std::chrono::milliseconds current = 0ms;
    std::chrono::milliseconds duration;
};

#endif // End of macro UTIL_TASK_ONE_SHOT_TASK_HPP