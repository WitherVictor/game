#pragma once

#include <atomic>
#include <cstddef>

class resource_type {
public:
    using value_type = std::size_t;
    using difference_type = long long;

    static constexpr value_type DEFAULT_MAXIMUM_POWER{25};

    struct state {
        value_type current;
        value_type maximum;
    };

    state get_state() {
        return state_.load();
    }

    bool try_recharge(value_type amount = 1) {
        return modify(amount);
    }

    bool try_comsume(value_type amount = 1) {
        return modify(-amount);
    }
private:
    bool modify(difference_type delta) {
        state expected{state_.load()};
        state desired{};

        do {
            if (delta > 0) {
                if (expected.maximum - expected.current < static_cast<value_type>(delta)) {
                    return false;
                }

                desired = {expected.current + static_cast<value_type>(delta), expected.maximum};
            } else {
                if (expected.current < static_cast<value_type>(-delta)) {
                    return false;
                }

                desired = {expected.current - static_cast<value_type>(-delta), expected.maximum};
            }
        } while (!state_.compare_exchange_weak(expected, desired));

        return true;
    }

    std::atomic<state> state_{{.current = 0, .maximum = DEFAULT_MAXIMUM_POWER}};
};