#ifndef VIEW_MODEL_SYSTEM_HPP
#define VIEW_MODEL_SYSTEM_HPP

#include <memory>

#include "model/electricity.hpp"
#include "util/atomic_clamped.hpp"

namespace view_model {
class system {
public:
    atomic_clamped<std::size_t>& get_electricity() {
        return electricity_->power;
    }
private:
    std::unique_ptr<electricity> electricity_ = std::make_unique<electricity>();
};
} // End of namespace view_model

#endif // End of macro VIEW_MODEL_SYSTEM_HPP