#pragma once

// STL
#include <cstddef>
#include <memory>

// Project
#include "model/task_manager.hpp"
#include "util/atomic_clamped.hpp"
#include "util/task/timer_task.hpp"
#include "inventory.hpp"

namespace model {
struct player {
    // 构造时，注册一部分任务
    player() {
        task_manager::instance().add_task(std::make_shared<timer_task>([this] {
            hunger.force_minus();
            thirst.force_minus();
        }, 1s, true));

        task_manager::instance().add_task(std::make_shared<timer_task>([this] {
            health.force_add();
        }, 60s, true));
    }

    // 玩家属性值
    atomic_clamped<std::size_t> health{0, 100, 100};
    atomic_clamped<std::size_t> hunger{0, 2500, 2000};
    atomic_clamped<std::size_t> thirst{0, 1500, 1200};

    // 玩家物品栏
    inventory inventory;
};
} // End of namespace model