#pragma once

#include <concepts>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>

template <typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

template <arithmetic T>
class atomic_clamped {
public:
    atomic_clamped(T min_, T now_, T max_)
        : min_{min_}, now_{now_}, max_{max_}
    {
        if (min_ > max_ || now_ < min_ || now_ > max_) {
            throw std::invalid_argument{"Invalid argument for atomic_clamped"};
        }
    }
    
    atomic_clamped(T min_, T max_)
        : atomic_clamped{min_, min_, max_} {}

    atomic_clamped(const atomic_clamped&) = delete;
    atomic_clamped& operator=(const atomic_clamped&) = delete;
    atomic_clamped(atomic_clamped&&) = default;
    atomic_clamped& operator=(atomic_clamped&&) = default;

    bool try_add(T delta = T{1}) {
        if (delta < 0) {
            return false;
        }

        std::unique_lock lock{mutex_};
        if (max_ - now_ < delta) {
            return false;
        }

        now_ += delta;

        return true;
    }

    bool try_minus(T delta = T{1}) {
        if (delta < 0) {
            return false;
        }

        std::unique_lock lock{mutex_};
        if (now_ - min_ < delta) {
            return false;
        }

        now_ -= delta;

        return true;
    }

    void force_add(T delta) {
        if (delta <= 0) {
            return;
        }

        std::unique_lock lock{mutex_};
        if (max_ - now_ >= delta) {
            now_ += delta;
        } else {
            now_ = max_;
        }
    }

    void force_minus(T delta) {
        if (delta <= 0) {
            return;
        }

        std::unique_lock lock{mutex_};
        if (now_ - min_ >= delta) {
            now_ -= delta;
        } else {
            now_ = min_;
        }
    }

    const T now() const {
        std::shared_lock lock{mutex_};
        return now_;
    }

    const T min() const {
        std::shared_lock lock{mutex_};
        return min_;
    }

    const T max() const {
        std::shared_lock lock{mutex_};
        return max_;
    }
private:
    T min_;
    T now_;
    T max_;

    mutable std::shared_mutex mutex_;
};