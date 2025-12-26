#pragma once

#include <cstddef>

namespace model {
class electricity {
public:
    static std::size_t current() {
        return current_;
    }
    
    static std::size_t maximum() {
        return maximum_;
    }
private:
    inline static std::size_t current_ = 0;
    inline static std::size_t maximum_ = 100;
};
}