#ifndef MODEL_INVENTORY_HPP
#define MODEL_INVENTORY_HPP

#include "item/item.hpp"
#include "item/factory.hpp"

#include <memory>
#include <ranges>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>

class inventory {
public:
    bool add_item(const std::string& id, const std::size_t amount = 1) {
        std::unique_lock lock{mutex_};

        auto result_iter = items_.find(id);
        if (result_iter == items_.end() && !is_full()) {
            items_[id] = item_factory::instance().create(id);
            if (items_[id]->stackable)
                items_[id]->amount = amount;
            else
                add_item(id, amount - 1);
            return true;
        } else if (result_iter != items_.end() ){
            items_[id]->amount += amount;
            return true;
        }

        return false;
    }

    bool remove_item(const std::string& id, const std::size_t amount = 1) {
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

    bool has_item(const std::string& id) const {
        std::shared_lock lock{mutex_};
        return items_.find(id) != items_.end();
    }

    bool has_item(const std::string& id, const std::size_t amount) const {
        std::shared_lock lock{mutex_};

        auto iter = items_.find(id);
        return iter != items_.end() && iter->second->amount >= amount;
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
    std::unordered_map<std::string, std::unique_ptr<item>> items_;
    std::size_t capacity_ = 8;
};

#endif