#pragma once

#include <concepts>
#include <stdexcept>

// 算术类型概念
template <typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

// 将值限定在给定的上下限的类型
// 要求底层类型必须为算术类型
template <typename T>
class clamped {
public:
    // 必须提供上下限
    clamped() = delete;

    // 默认当前值为最小值
    clamped(T min, T max)
        : clamped{min, max, min} {}

    clamped(T min, T max, T now)
        : min_{min}, max_{max}, now_{now}
    {
        // 如果最小值大于最大值，则抛出运行时异常
        if (min_ > max_) {
            throw std::invalid_argument{"min_ cannot be larger than max_ in clamped"};
        }
    }

    const T min() const {
        return min_;
    }

    const T max() const {
        return max_;
    }

    const T now() const {
        return now_;
    }

    // 设置最小值
    void set_min(T new_min) {
        min_ = new_min;
    }

    // 设置最大值
    void set_max(T new_max) {
        max_ = new_max;
    }

    // 设置当前值
    bool set_now(T new_now) {
        // 如果给定值超出范围
        // 则直接返回失败
        if (new_now < min_ || new_now > max_) {
            return false;
        }

        // 否则返回成功
        now_ = new_now;
        return true;
    }

    // 尝试增加值
    bool try_add(T delta = T{1}) {
        // 值不可为负
        if (delta < 0) {
            return false;
        }

        // 如果有足够的差值
        // 则增加值并返回成功
        if (max_ - now_ > delta) {
            now_ += delta;
            return true;
        }

        // 否则返回失败
        return false;
    }

    // 强制增加值
    void force_add(T delta = T{1}) {
        // 值不可为负
        if (delta < 0) {
            return;
        }

        // 如果有足够的差值供增加
        if (max_ - now_ > delta) {
            now_ += delta; // 那么增加值
        } else {
            now_ = max_;   // 否则设置值为上限
        }
    }

    // 尝试减少值
    bool try_minus(T delta = T{1}) {
        // 值不可为负
        if (delta < 0) {
            return false;
        }

        // 如果有足够的差值供减少
        // 那么减少值并返回成功
        if (now_ - min_ > delta) {
            now_ -= delta;
            return true;
        }

        // 否则返回失败
        return false;
    }

    void force_minus(T delta = T{1}) {
        // 值不可为负
        if (delta < 0) {
            return;
        }

        // 如果有足够的差值供减少
        if (now_ - min_ > delta) {
            now_ -= delta; // 那么直接减少当前值
        } else {
            now_ = min_;   // 否则设置当前值为最小值
        }
    }
private:
    // 最小值与最大值
    T min_;
    T max_;

    // 当前值
    T now_;
};