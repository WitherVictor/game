#pragma once

// STL
#include <cstddef>

// Project
#include "../model/ore.hpp"

namespace view_model {
class ore {
public:
    static std::size_t min() { return model::ore.min(); }
    static std::size_t max() { return model::ore.max(); }
    static std::size_t now() { return model::ore.now(); }

    static double ratio() {
        auto now_ = now();
        auto max_ = max();

        return static_cast<double>(now_) / max_;
    }

    static void set_miner_state(bool status) {
        model::solar_to_iron.set_state(status);
    }
};
} // End of namespace view_model