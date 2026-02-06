#ifndef VIEW_MODEL_PLAYER_HPP
#define VIEW_MODEL_PLAYER_HPP

#include <cstddef>
#include <memory>

#include "model/player.hpp"
#include "util/atomic_clamped.hpp"

#include "item/attribute/nutrition.hpp"
#include "item/attribute/hydration.hpp"

struct player_info {
    clamped_values<std::size_t> health;
    clamped_values<std::size_t> hunger;
    clamped_values<std::size_t> thirst;
};

namespace view_model {
class player {
public:
    player_info get_player_info() {
        return {
            player_->health.values(),
            player_->hunger.values(),
            player_->thirst.values()
        };
    }

    auto get_inventory_items() {
        return player_->inventory.get_items();
    }

    bool try_consume_hunger(const std::size_t delta = 1) {
        return player_->hunger.try_minus(delta);
    }

    void dig_ice() {
        player_->hunger.force_minus(10);
        player_->inventory.add_item("ice_block");
    }

    void collect_metal_scrap() {
        player_->hunger.force_minus(25);
        player_->inventory.add_item("metal_scrap");
    }

    void use(const item* item)  {
        if (!player_->inventory.has_item(item->id))
            return;
        
        if (item->has_attribute<attribute_nutrition>())
            eat(item);

        if (item->has_attribute<attribute_hydration>())
            drink(item);
    }

    void eat(const item* item) {
        auto item_attr = item->get_attribute<attribute_nutrition>();
        player_->hunger.force_add(item_attr->nutrition);
        player_->inventory.remove_item(item->id);
    }

    void drink(const item* item) {
        auto item_attr = item->get_attribute<attribute_hydration>();
        player_->thirst.force_add(item_attr->hydration);
        player_->inventory.remove_item(item->id);
    }
private:
    std::unique_ptr<model::player> player_ = std::make_unique<model::player>();
};
} // End of namespace view_model

#endif // End of macro VIEW_MODEL_PLAYER_HPP