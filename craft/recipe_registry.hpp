#ifndef CRAFT_RECIPE_REGISTRY_HPP
#define CRAFT_RECIPE_REGISTRY_HPP

#include <string>
#include <stdexcept>
#include <unordered_map>

#include "model/craft/recipe.hpp"

class recipe_registry {
public:
    static recipe_registry& instance() {
        static recipe_registry instance;
        return instance;
    }

    void register_recipe(recipe new_recipe) {
        recipes[new_recipe.id] = new_recipe;
    }

    const recipe* get(std::string id) const {
        auto iter = recipes.find(id);
        if (iter != recipes.end())
            return &iter->second;

        throw std::runtime_error{"Recipe registry trying to get non-exist recipe! Recipe ID: " + id};
    }
private:
    std::unordered_map<std::string, recipe> recipes;
};

#endif // End of macro CRAFT_RECIPE_REGISTRY_HPP