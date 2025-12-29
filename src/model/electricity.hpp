#pragma once

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <thread>
#include <atomic>

#include <ftxui/component/component.hpp>

#include "../util/atomic_clamped.hpp"

namespace model {
using namespace ftxui;

namespace {
class electricity_impl {
public:
    electricity_impl() {
        solar_ = std::jthread([this](std::stop_token token) {
            using namespace std::chrono_literals;

            while (!token.stop_requested()) {
                std::unique_lock lock{solar_mutex_};
                solar_cv_.wait(lock, [this] { return is_on(); });

                std::this_thread::sleep_for(2.5s);
                value_.try_add();
            }
        });
    }

    electricity_impl(const electricity_impl&) = delete;
    electricity_impl& operator=(const electricity_impl&) = delete;
    electricity_impl(electricity_impl&&) = delete;
    electricity_impl& operator=(electricity_impl&&) = delete;

    bool is_on() const {
        return solar_is_on;
    }

    std::size_t min() const { return value_.min(); }
    std::size_t max() const { return value_.max(); }
    std::size_t now() const { return value_.now(); }
private:
    atomic_clamped<std::size_t> value_{0, 100};

    std::jthread solar_;
    std::condition_variable solar_cv_;
    std::mutex solar_mutex_;
    std::atomic<bool> solar_is_on = true;
};
}

inline electricity_impl electricity;
}