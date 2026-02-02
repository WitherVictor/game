#ifndef MODEL_INVENTORY_HPP
#define MODEL_INVENTORY_HPP

#include "item/item.hpp"

#include <map>
#include <memory>
#include <ranges>

class inventory {
public:
    bool add_item(std::shared_ptr<item> item_ptr) {
        if (items.size() >= capacity_)
            return false;

        auto iter = items.find(item_ptr->get_name());
        if (iter != items.end()) {
            iter->second->add(item_ptr->get_amount());
        } else {
            items[item_ptr->get_name()] = item_ptr;
        }

        return true;
    }

    auto get_items() const {
        return items
                | std::views::transform([] (const auto& pair) { return pair.second; })
                | std::ranges::to<std::vector<std::shared_ptr<item>>>();
    }
private:
    std::map<std::string, std::shared_ptr<item>> items;
    std::size_t capacity_ = 8;
};

#endif