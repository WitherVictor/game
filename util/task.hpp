#pragma once

#include <chrono>
#include <concepts>
#include <functional>
#include <mutex>
#include <shared_mutex>

using namespace std::chrono_literals;

class task {
public:
    template <typename rep, typename duration>
    task(std::function<void()> func, std::chrono::duration<rep, duration> task_duration)
        : task_{func}, duration_{std::chrono::duration_cast<std::chrono::milliseconds>(task_duration)} {}

    void update(const std::chrono::milliseconds& elapsed) {
        std::unique_lock lock{mutex_};
        if (condition_) {
            current_ += elapsed;
            if (current_ >= duration_) {
                current_ -= duration_;
                lock.unlock();
                std::invoke(task_);
            }
        }
    }

    //  修改
    void set_duration(const std::chrono::milliseconds& new_duration) {
        std::unique_lock lock{mutex_};
        duration_ = new_duration;
    }

    void reverse_condition() {
        condition_ ? stop() : start();
    }

    void start() {
        std::unique_lock lock{mutex_};
        condition_ = true;
        current_ = 0ms;
    }

    void stop() {
        std::unique_lock lock{mutex_};
        condition_ = false;
        current_ = 0ms;
    }

    // 访问
    double progress() const {
        std::shared_lock lock{mutex_};
    
        auto current = current_.count();
        auto maximum = duration_.count();

        return static_cast<double>(current) / maximum;
    }
private:
    mutable std::shared_mutex mutex_;

    bool condition_ = false;

    std::function<void()> task_;

    std::chrono::milliseconds current_ = 0ms;
    std::chrono::milliseconds duration_;
};