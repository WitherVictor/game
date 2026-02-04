#ifndef ITEMS_ITEM_HPP
#define ITEMS_ITEM_HPP

#include <string>

#include "item_id.hpp"
#include "item_category.hpp"

// 代表一个物品
// 拥有名称，说明和数量
struct item {
    item(item_id id, const std::string& name, const std::string& description, const std::size_t amount)
        : id{id}, name{name}, description{description}, amount{amount} {}

    virtual item_category get_type() const {
        return item_category::none;
    }

    bool is_food() const { return get_type() == item_category::food; };
    bool is_drink() const { return get_type() == item_category::drink; };

    item_id id = item_id::item;
    std::string name = "物品";
    std::string description = "按理来说你不应该看见这个。";
    std::size_t amount = 1;
};

// 食物
struct item_food : public item {
    item_food(item_id id, const std::string& name, const std::string& description, const std::size_t amount, const std::size_t nutrition)
        : item{id, name, description, amount}, nutrition{nutrition} {}

    virtual item_category get_type() const override {
        return item_category::food;
    }

    std::size_t nutrition;
};

// 饮品
struct item_drink : public item {
    item_drink(item_id id, const std::string& name, const std::string& description, const std::size_t amount, const std::size_t hydration)
        : item{id, name, description, amount}, hydration{hydration} {}

    virtual item_category get_type() const override {
        return item_category::drink;
    }

    std::size_t hydration;
};

#endif // End of macro ITEMS_ITEM_HPP