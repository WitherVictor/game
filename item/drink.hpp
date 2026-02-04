#ifndef ITEM_DRINK_HPP
#define ITEM_DRINK_HPP

#include "item/register_macro.hpp"

REGISTER_ITEM_DRINK(
    item_id::ice_block,
    ice_block,
    "冰块",
    "一颗内部含有一些杂质的冰块，"
    "为了安全着想，最好融化煮沸后饮用。",
    100
);

#endif // End of macro ITEM_DRINK_HPP