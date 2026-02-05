#ifndef MODEL_INVENTORY_HPP
#define MODEL_INVENTORY_HPP

#include "item/item.hpp"
#include "item/item_id.hpp"
#include "item/item_factory.hpp"

#include <map>
#include <memory>
#include <ranges>
#include <mutex>
#include <shared_mutex>

class inventory {
public:
    bool add_item(item_id id, const std::size_t amount = 1) {
        std::unique_lock lock{mutex_};
        auto result_iter = items_.find(id);
        if (result_iter == items_.end() && !is_full()) {
            items_[id] = item_factory::instance().create(id, amount);
            return true;
        } else if (result_iter != items_.end() ){
            items_[id]->amount += amount;
            return true;
        }

        return false;
    }

    bool remove_item(item_id id, const std::size_t amount = 1) {
        std::unique_lock lock{mutex_};
        auto result_iter = items_.find(id);
        if (result_iter != items_.end() && result_iter->second->amount >= amount) {
            result_iter->second->amount -= amount;

            if (result_iter->second->amount == 0)
                items_.erase(result_iter);

            return true;
        } else {
            return false;
        }
    }

    bool has_item(item_id id) {
        std::shared_lock lock{mutex_};
        return items_.find(id) != items_.end();
    }

    bool is_full() const {
        return items_.size() == capacity_;
    }

    const auto get_items() const {
        std::shared_lock lock{mutex_};
        return items_
                | std::views::transform([] (const auto& pair) { return pair.second.get(); })
                | std::ranges::to<std::vector<const item*>>();
    }
private:
    mutable std::shared_mutex mutex_;
    std::map<item_id, std::unique_ptr<item>> items_;
    std::size_t capacity_ = 8;
};

#endif