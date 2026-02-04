#ifndef ITEM_REGISTER_MACRO_HPP
#define ITEM_REGISTER_MACRO_HPP

#include "item/item_factory.hpp"

#define AUTO_ITEM_REGISTER(item_id, class_name) \
class class_name##_register {                                    \
public:                                                          \
    class_name##_register() {                                    \
        item_factory::instance()                                 \
            .register_creator(                                   \
                item_id,                                         \
                [](std::size_t amount = 1) {                     \
                    return std::make_unique<class_name>(amount); \
                }                                                \
            );                                                   \
    }                                                            \
};                                                               \
static inline class_name##_register class_name##_reg;            \

#define REGISTER_ITEM(item_id, class_name, item_name, item_description) \
class class_name : public item {                                        \
public:                                                                 \
    class_name(std::size_t amount = 1)                                  \
        : item {                                                        \
            (item_id),                                                  \
            (item_name),                                                \
            (item_description),                                         \
            amount                                                      \
        } {}                                                            \
private:                                                                \
    AUTO_ITEM_REGISTER(item_id, class_name);                            \
}                                                                       \

#define REGISTER_ITEM_FOOD(item_id, class_name, item_name, item_description, item_nutrition) \
class class_name : public item_food {                                                        \
public:                                                                                      \
    class_name(std::size_t amount = 1)                                                       \
        : item_food {                                                                        \
            (item_id),                                                                       \
            (item_name),                                                                     \
            (item_description),                                                              \
            amount,                                                                          \
            (item_nutrition)                                                                 \
        } {}                                                                                 \
private:                                                                                     \
    AUTO_ITEM_REGISTER(item_id, class_name);                                                 \
}                                                                                            \

#define REGISTER_ITEM_DRINK(item_id, class_name, item_name, item_description, item_hydration) \
class class_name : public item_drink {                                                        \
public:                                                                                       \
    class_name(std::size_t amount = 1)                                                        \
        : item_drink {                                                                        \
            (item_id),                                                                        \
            (item_name),                                                                      \
            (item_description),                                                               \
            amount,                                                                           \
            (item_hydration)                                                                  \
        } {}                                                                                  \
private:                                                                                      \
    AUTO_ITEM_REGISTER(item_id, class_name);                                                  \
}                                                                                             \

#endif // End of macro ITEM_REGISTER_MACRO_HPP