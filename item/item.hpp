#ifndef ITEM_ITEM_HPP
#define ITEM_ITEM_HPP

#include <concepts>
#include <string>
#include <vector>
#include <memory>

#include "attribute/attribute.hpp"
#include "item/attribute/attribute.hpp"

struct item {
    template <std::derived_from<attribute> T>
    T* get_attribute() const {
        for (auto& attribute : attributes) {
            if (auto ptr = dynamic_cast<T*>(attribute.get()))
                return ptr;
        }

        return nullptr;
    }

    template <std::derived_from<attribute> T>
    bool has_attribute() const {
        return get_attribute<T>() != nullptr;
    }

    std::string id;
    std::string name;
    std::string description;
    bool stackable;
    std::size_t amount;
    std::vector<std::unique_ptr<attribute>> attributes;
};

#endif // End of macro ITEM_ITEM_HPP