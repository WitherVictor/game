#ifndef MODEL_TASK_MANAGER_HPP
#define MODEL_TASK_MANAGER_HPP

#include <chrono>
#include <vector>
#include <memory>
#include <mutex>
#include <shared_mutex>

#include "util/task/task.hpp"

class task_manager {
public:
    static task_manager& instance() {
        static task_manager instance;
        return instance;
    }

    void add_task(std::shared_ptr<task> task_ptr) {
        std::unique_lock lock{mutex_};
        observers_.push_back(task_ptr);
    }

    void update_all(std::chrono::milliseconds elapsed_time) {
        std::shared_lock lock{mutex_};
        
        std::erase_if(observers_, [elapsed_time](const auto& observer) {
            return observer->update(elapsed_time) == false;
        });
    }

private:
    mutable std::shared_mutex mutex_;
    std::vector<std::shared_ptr<task>> observers_;
};

#endif // End of macro MODEL_TASK_MANAGER_HPP