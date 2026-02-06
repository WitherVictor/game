#ifndef ITEM_ATTRIBUTE_STACKABLE_HPP
#define ITEM_ATTRIBUTE_STACKABLE_HPP

#include <nlohmann/json.hpp>

#include <string>

#include "attribute.hpp"

struct attribute_stackable : public attribute {
    std::string get_name() const override {
        return "stackable";
    }

    void from_json(const nlohmann::json& json) override {
        max_amount = json.at(get_name()).get<std::size_t>();
    }

    // 创建出来后最少有一个物品
    std::size_t amount = 1;
    std::size_t max_amount = 0;
};

#endif // End of macro ITEM_ATTRIBUTE_STACKABLE_HPP