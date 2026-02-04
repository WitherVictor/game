#ifndef VIEW_MODEL_SYSTEM_HPP
#define VIEW_MODEL_SYSTEM_HPP

#include <memory>

#include "model/electricity.hpp"
#include "util/atomic_clamped.hpp"

namespace view_model {
class system {
public:
    auto get_electricity_values() {
        return electricity_->power.values();
    }

    void force_restore_power(const std::size_t delta = 1) {
        electricity_->power.force_add(delta);
    }
private:
    std::unique_ptr<electricity> electricity_ = std::make_unique<electricity>();
};
} // End of namespace view_model

#endif // End of macro VIEW_MODEL_SYSTEM_HPP