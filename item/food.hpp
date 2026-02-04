#ifndef ITEM_FOOD_HPP
#define ITEM_FOOD_HPP

#include "item_id.hpp"
#include "item/register_macro.hpp"

REGISTER_ITEM_FOOD(
    item_id::food,
    protein_bar,
    "蛋白棒",
    "一根含有人体所需营养元素的蛋白棒, "
    "一日三根就能保证一个成年人的正常消耗",
    800
);

#endif // End of macro ITEM_FOOD_HPP