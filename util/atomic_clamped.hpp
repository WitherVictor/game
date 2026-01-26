#pragma once

#include <mutex>
#include <shared_mutex>

#include "util/clamped.hpp"

template <typename T>
class atomic_clamped {
public:
    // 必须提供上下限
    atomic_clamped() = delete;

    atomic_clamped(T min, T max, T now)
        : value_{min, max, now} {}

    atomic_clamped(T min, T max)
        : value_{min, max} {}

    // 互斥锁不可复制
    atomic_clamped(const atomic_clamped&) = delete;
    atomic_clamped& operator=(const atomic_clamped&) = delete;

    const T min() const {
        std::shared_lock lock{mutex_};
        return value_.min();
    }

    const T max() const {
        std::shared_lock lock{mutex_};
        return value_.max();
    }

    const T now() const {
        std::shared_lock lock{mutex_};
        return value_.now();
    }

    const long double ratio() const {
        std::shared_lock lock{mutex_};
        return value_.ratio();
    }

    const clamped<T>::info values() const {
        std::shared_lock lock{mutex_};
        return value_.values();
    }

    void set_min(T new_min) {
        std::unique_lock lock{mutex_};
        value_.set_min(new_min);
    }

    void set_max(T new_max) {
        std::unique_lock lock{mutex_};
        value_.set_max(new_max);
    }

    bool set_now(T new_now) {
        std::unique_lock lock{mutex_};
        return value_.set_now(new_now);
    }

    bool try_add(T delta = T{1}) {
        std::unique_lock lock{mutex_};
        return value_.try_add(delta);
    }

    void force_add(T delta = T{1}) {
        std::unique_lock lock{mutex_};
        value_.force_add(delta);
    }

    bool try_minus(T delta = T{1}) {
        std::unique_lock lock{mutex_};
        return value_.try_minus(delta);
    }

    void force_minus(T delta = T{1}) {
        std::unique_lock lock{mutex_};
        value_.force_minus(delta);
    }
private:
    clamped<T> value_;
    mutable std::shared_mutex mutex_;
};
