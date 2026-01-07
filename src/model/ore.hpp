#pragma once

// STL
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <stop_token>
#include <thread>

// Project
#include "../util/atomic_clamped.hpp"
#include "electricity.hpp"

namespace model {
namespace {
class ore_impl {
public:
    ore_impl() = default;

    // 独占资源禁止复制和移动
    ore_impl(const ore_impl&) = delete;
    ore_impl& operator=(const ore_impl&) = delete;
    ore_impl(ore_impl&&) = delete;
    ore_impl& operator=(ore_impl&&) = delete;

    // 元素访问
    std::size_t min() const { return value_.min(); }
    std::size_t max() const { return value_.max(); }
    std::size_t now() const { return value_.now(); }

    bool try_produce(std::size_t delta = 1) {
        return value_.try_add(delta);
    }
private:
    atomic_clamped<std::size_t> value_{0, 100};
};

class solar_to_iron_impl {
public:
    solar_to_iron_impl(ore_impl& singleton_ore)
        : ore_{singleton_ore}
    {
        thread_ = std::jthread([this](std::stop_token token) {
            using namespace std::chrono_literals;

            while (!token.stop_requested()) {
                std::unique_lock lock{mutex_};
                cv_.wait(lock, [this, &token] {
                    return is_mining_ || token.stop_requested();
                });

                if (token.stop_requested())
                    return;

                // 等待 10s 后挖到矿石
                std::this_thread::sleep_for(10s);

                // 如果时间到仍然开启，那么进行资源转换
                if (is_mining_ && electricity.try_consume(10)) {
                    ore_.try_produce();
                }
            }
        });
    }

    void set_state(bool status) {
        is_mining_ = status;
        cv_.notify_one();
    }
private:
    std::jthread thread_;
    std::condition_variable cv_;
    std::mutex mutex_;
    std::atomic_bool is_mining_ = false;

    ore_impl& ore_;
};
} // End of anomynous namespace

inline ore_impl ore;
inline solar_to_iron_impl solar_to_iron{ore};
} // End of namespace model