#ifndef CRAFT_CRAFT_SYSTEM_HPP
#define CRAFT_CRAFT_SYSTEM_HPP

#include "model/craft/recipe.hpp"
#include "model/inventory.hpp"
#include "util/task.hpp"

class craft_system {
public:
    bool can_craft(const recipe& recipe, const inventory& inventory) {
        for (const auto& ingredient : recipe.inputs) {
            if (!inventory.has_item(ingredient.item_id, ingredient.amount))
                return false;
        }

        return true;
    }

    void craft(const recipe& recipe, inventory& inventory, task& task) {
        // 移除配方所需物品
        for (const auto& ingredient : recipe.inputs)
            inventory.remove_item(ingredient.item_id, ingredient.amount);

        // 如果配方不耗时
        // 则直接将物品添加到物品栏内
        if (recipe.duration == 0ms)
            for (const auto& product : recipe.outputs)
                inventory.add_item(product.item_id, product.amount);

        // 否则创建任务
        auto task_ptr = std::make_shared<task>([&recipe, &inventory] {
            for (const auto& product : recipe.outputs)
                inventory.add_item(product.item_id, product.amount);
        }, recipe.duration);
    }
};

#endif // End of macro CRAFT_CRAFT_SYSTEM_HPP