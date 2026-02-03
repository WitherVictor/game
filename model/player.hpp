#pragma once

// STL
#include <cstddef>
#include <memory>

// Project
#include "model/task_manager.hpp"
#include "util/atomic_clamped.hpp"

struct player {
    player() {
        task_manager::instance().add_task(std::make_shared<task>([this] {
            hunger.force_minus();
            thirst.force_minus();
        }, 1s, true));

        task_manager::instance().add_task(std::make_shared<task>([this] {
            health.force_add();
        }, 60s, true));
    }

    atomic_clamped<std::size_t> health{0, 100, 100};
    atomic_clamped<std::size_t> hunger{0, 2500, 2000};
    atomic_clamped<std::size_t> thirst{0, 1500, 1200};
};