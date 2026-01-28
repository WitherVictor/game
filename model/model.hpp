#pragma once

#include <chrono>
#include <list>
#include <memory>

#include "impl/electricity.hpp"
#include "impl/player.hpp"

#include "util/task.hpp"

class model {
public:
    void update_all(std::chrono::milliseconds elapsed_time) {
        for (auto& observer : observers_) {
            observer->update(elapsed_time);
        }
    }

    void register_task(std::shared_ptr<task> task_ptr) {
        observers_.push_back(task_ptr);
    }

    static model& instance() {
        static model instance_;
        return instance_;
    }

    static player_impl& player() {
        static player_impl instance;
        return instance;
    }

    static electricity_impl& electricity() {
        static electricity_impl instance;
        return instance;
    }
private:
    std::list<std::shared_ptr<task>> observers_;
};