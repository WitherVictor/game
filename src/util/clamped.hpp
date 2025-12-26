#pragma once

template <typename T>
class clamped {
public:
    clamped(T min_, T now_, T max_)
        : min_{min_}, now_{now_}, max_{max_} {}
    
    clamped(T min_, T max_)
        : clamped{min_, min_, max_} {}

    bool try_add(T delta) {
        if (delta < 0) {
            return false;
        }

        if (max_ - delta >= now_) {
            now_ += delta;
            return true;
        }

        return false;
    }

    bool try_minus(T delta) {
        if (delta < 0) {
            return false;
        }

        if (now_ - min_ >= delta) {
            now_ -= delta;
            return true;
        }

        return false;
    }

    void force_add(T delta) {
        if (delta <= 0) {
            return;
        }

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

        if (now_ - min_ >= delta) {
            now_ -= delta;
        } else {
            now_ = min_;
        }
    }

    const T now() const {
        return now_;
    }

    const T min() const {
        return min_;
    }

    const T max() const {
        return max_;
    }
private:
    T min_;
    T max_;

    T now_;
};