#ifndef ITEM_ATTRIBUTE_FACTORY_HPP
#define ITEM_ATTRIBUTE_FACTORY_HPP

#include <memory>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "attribute.hpp"

// 属性创建工厂
// 根据给定 ID 创建 attribute
class attribute_factory {
public:
    using creator = std::function<std::unique_ptr<attribute>()>;

    // 单例模式
    static attribute_factory& instance() {
        static attribute_factory instance;
        return instance;
    }

    // 运行时注册创建函数
    void register_creator(const std::string& id, creator func) {
        creators[id] = std::move(func);
    }

    // 工厂函数，创建属性
    std::unique_ptr<attribute> create(const std::string& id) {
        auto iter = creators.find(id);
        if (iter != creators.end()) {
            return iter->second();
        } else {
            throw std::runtime_error{"Attribute factory tring to create non-exist attribute! Attribute ID: " + id};
        }
    }
private:
    // 属性创建方法列表
    std::unordered_map<std::string, creator> creators;
};

#endif // End of macro ITEM_ATTRIBUTE_FACTORY_HPP