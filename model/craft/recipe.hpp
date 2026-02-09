#ifndef MODEL_CRAFT_RECIPE_HPP
#define MODEL_CRAFT_RECIPE_HPP

#include <chrono>
#include <cstddef>
#include <string>
#include <vector>

// 合成需要的物品和产出的物品类型
struct ingredient {
    std::string item_id;
    std::size_t amount;
};

struct recipe {
    std::string id;
    std::string name;

    std::vector<ingredient> inputs;
    std::vector<ingredient> outputs;

    std::chrono::milliseconds duration;
};

#endif // End of macro MODEL_CRAFT_RECIPE_HPP