#ifndef ITEM_ATTRIBUTE_HYDRATION_HPP
#define ITEM_ATTRIBUTE_HYDRATION_HPP

// Json
#include <nlohmann/json.hpp>

#include "attribute.hpp"

struct attribute_hydration : public attribute {
    std::string get_name() const override {
        return "hydration";
    }

    void from_json(const nlohmann::json& json) override {
        hydration = json.at(get_name()).get<std::size_t>();
    }

    std::size_t hydration = 0;
};

#endif // End of macro ITEM_ATTRIBUTE_HYDRATION_HPP