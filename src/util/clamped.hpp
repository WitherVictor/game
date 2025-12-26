#pragma once

template <typename T>
class clamped {
public:
    clamped(T min, T now, T max)
        : min{min}, now{now}, max{max} {}
    
    clamped(T min, T max)
        : clamped{min, min, max} {}

    bool try_add(T delta) {
        if (delta < 0) {
            return false;
        }

        if (max - delta >= now) {
            now += delta;
            return true;
        }

        return false;
    }

    bool try_minus(T delta) {
        if (delta < 0) {
            return false;
        }

        if (now - min >= delta) {
            now -= delta;
            return true;
        }

        return false;
    }

    void force_add(T delta) {
        if (delta <= 0) {
            return;
        }

        if (max - now >= delta) {
            now += delta;
        } else {
            now = max;
        }
    }

    void force_minus(T delta) {
        if (delta <= 0) {
            return;
        }

        if (now - min >= delta) {
            now -= delta;
        } else {
            now = min;
        }
    }

    const T get_now() const {
        return now;
    }

    const T get_min() const {
        return min;
    }

    const T get_max() const {
        return max;
    }
private:
    T min;
    T max;

    T now;
};