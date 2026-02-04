#ifndef ITEM_ITEM_FACTORY_HPP
#define ITEM_ITEM_FACTORY_HPP

#include <memory>
#include <stdexcept>
#include <functional>
#include <unordered_map>

#include "item.hpp"
#include "item_id.hpp"

class item_factory {
public:
    using creator = std::function<std::unique_ptr<item>(std::size_t)>;

    static item_factory& instance() {
        static item_factory instance;
        return instance;
    }

    std::unique_ptr<item> create(item_id id, const std::size_t amount = 1) {
        auto iter = registry_.find(id);
        if (iter != registry_.end())
            return iter->second(amount);

        throw std::runtime_error{"Factory trying to create item that hasn't register!"};
    }

    void register_creator(item_id id, creator func) {
        registry_[id] = func;
    }

private:
    item_factory() = default;

    std::unordered_map<item_id, creator> registry_;
};

#endif // End of macro ITEM_ITEM_FACTORY_HPP