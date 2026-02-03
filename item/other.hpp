#ifndef ITEM_OTHER_HPP
#define ITEM_OTHER_HPP

#include "item.hpp"
#include "model/model.hpp"

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

#endif // End of macro ITEM_OTHER_HPP