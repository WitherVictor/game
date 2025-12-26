#pragma once

#include <cstddef>

#include "model/electricity.hpp"

namespace view_model {
class electricity{
public:
    static std::size_t current() {
        return model::electricity::current();
    }

    static std::size_t maximum() {
        return model::electricity::maximum();
    }

    static double ratio() {
        auto current_ = current();
        auto maximum_ = maximum();

        return static_cast<double>(current_) / maximum_;
    }
};
}