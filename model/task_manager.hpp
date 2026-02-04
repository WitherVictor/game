#ifndef MODEL_TASK_MANAGER_HPP
#define MODEL_TASK_MANAGER_HPP

#include <chrono>
#include <vector>
#include <memory>
#include <mutex>

#include "util/task.hpp"

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
        for (auto& observer : observers_) {
            observer->update(elapsed_time);
        }
    }

private:
    mutable std::shared_mutex mutex_;
    std::vector<std::shared_ptr<task>> observers_;
};

#endif // End of macro MODEL_TASK_MANAGER_HPP