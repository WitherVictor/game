#ifndef ITEMS_FOOD_PROTEIN_BAR_HPP
#define ITEMS_FOOD_PROTEIN_BAR_HPP

#include <string>
#include <functional>

#include "item/item.hpp"
#include "model/model.hpp"

class protein_bar : public item {
public:
    protein_bar(std::size_t amount)
        : item{"蛋白棒", amount, [] {
            model::instance().player.hunger.force_add(hunger_add_count);
        }} {}

    virtual std::string get_description() const override {
        return "一根含有人体所需营养元素的蛋白棒, 一日三根就能保证一个成年人的正常消耗";
    }
private:
    inline static const std::size_t hunger_add_count = 800;
};

#endif // End of macro ITEMS_FOOD_PROTEIN_BAR_HPP