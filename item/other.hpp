#ifndef ITEM_OTHER_HPP
#define ITEM_OTHER_HPP

#include "item.hpp"
#include "model/model.hpp"
#include <cstddef>

class ice_block : public usable_item {
public:
    ice_block(std::size_t amount)
        : usable_item{
            "冰块",
            amount,
            "一颗内部含有一些杂质的冰块，"
            "为了安全着想，最好融化煮沸后饮用。",
            [] {
                auto& player = model::instance().player;

                player.health.force_minus(5);
                player.hunger.force_minus(10);
                player.thirst.force_add(100);
            }
        } {}
};

class metal_scrap : public item {
public:
    metal_scrap(std::size_t amount)
        : item{
            "金属碎片",
            amount,
            "一片较小的金属碎片，"
            "非常适合在适量的敲打下制作成其他物品。",
        } {}
};

#endif // End of macro ITEM_OTHER_HPP