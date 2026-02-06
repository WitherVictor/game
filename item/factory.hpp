#ifndef ITEM_FACTORY_HPP
#define ITEM_FACTORY_HPP

#include <string>
#include <unordered_map>

#include "item.hpp"

class item_factory {
public:
    using creator = std::function<std::unique_ptr<item>()>;

    static item_factory& instance() {
        static item_factory instance;
        return instance;
    }

    // 注册物品创建函数
    void register_creator(const std::string& id, creator func) {
        items[id] = std::move(func);
    }

    // 创建物品
    std::unique_ptr<item> create(const std::string& id) {
        auto iter = items.find(id);
        if (iter != items.end())
            return iter->second();

        throw std::runtime_error{"Item factory tring to create non-exist item! Item ID: " + id};
    }
private:
    std::unordered_map<std::string, creator> items;
};

#endif // End of macro ITEM_FACTORY_HPP