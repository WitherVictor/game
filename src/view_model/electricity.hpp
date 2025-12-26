#pragma once

#include <cstddef>

#include "../model/electricity.hpp"

namespace view_model {
class electricity {
public:
    static std::size_t min() { return model::electricity.min(); }
    static std::size_t max() { return model::electricity.max(); }
    static std::size_t now() { return model::electricity.now(); }

    static double ratio() {
        auto now_ = now();
        auto max_ = max();

        return static_cast<double>(now_) / max_;
    }
};
}