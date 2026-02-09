#ifndef UTIL_TASK_HPP
#define UTIL_TASK_HPP

#include <chrono>

class task {
public:
    virtual bool update(std::chrono::milliseconds) = 0;
    virtual long double progress() const = 0;

    virtual ~task() = default;
};

#endif // End of macro UTIL_TASK_HPP