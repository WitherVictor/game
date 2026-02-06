#ifndef ITEM_ATTRIBUTE_NUTRITION_HPP
#define ITEM_ATTRIBUTE_NUTRITION_HPP

// Json
#include <nlohmann/json.hpp>

#include "attribute.hpp"

struct attribute_nutrition : public attribute {
    std::string get_name() const override {
        return "nutrition";
    }

    void from_json(const nlohmann::json& json) override {
        nutrition = json.at(get_name()).get<std::size_t>();
    }

    std::size_t nutrition = 0;
};

#endif // End of macro ITEM_ATTRIBUTE_NUTRITION_HPP