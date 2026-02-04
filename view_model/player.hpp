#ifndef VIEW_MODEL_PLAYER_HPP
#define VIEW_MODEL_PLAYER_HPP

#include <cstddef>
#include <memory>

#include "item/item_id.hpp"
#include "model/player.hpp"
#include "util/atomic_clamped.hpp"

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
        player_->inventory.add_item(item_id::ice_block);
    }

    void collect_metal_scrap() {
        player_->hunger.force_minus(25);
        player_->inventory.add_item(item_id::metal_scrap);
    }

    void use(const item* item)  {
        if (item->is_food())
            eat(dynamic_cast<const item_food*>(item));
        else if (item->is_drink())
            drink(dynamic_cast<const item_drink*>(item));
    }

    void eat(const item_food* item) {
        if (player_->inventory.has_item(item->id)) {
            player_->hunger.force_add(item->nutrition);
            player_->inventory.remove_item(item->id);
        }
    }

    void drink(const item_drink* item) {
        if (player_->inventory.has_item(item->id)) {
            player_->thirst.force_add(item->hydration);
            player_->inventory.remove_item(item->id);
        }
    }
private:
    std::unique_ptr<model::player> player_ = std::make_unique<model::player>();
};
} // End of namespace view_model

#endif // End of macro VIEW_MODEL_PLAYER_HPP