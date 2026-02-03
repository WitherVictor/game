#ifndef ITEMS_FOOD_PROTEIN_BAR_HPP
#define ITEMS_FOOD_PROTEIN_BAR_HPP

#include <string>
#include <functional>

#include "item.hpp"
#include "model/model.hpp"

class protein_bar : public usable_item {
public:
    protein_bar(const std::size_t amount)
        : usable_item{
            "蛋白棒",
            amount,
            "一根含有人体所需营养元素的蛋白棒, 一日三根就能保证一个成年人的正常消耗",
            [] {
                model::instance().player.hunger.force_add(hunger_add_count);
            }
        } {}
private:
    inline static const std::size_t hunger_add_count = 800;
};

#endif // End of macro ITEMS_FOOD_PROTEIN_BAR_HPP