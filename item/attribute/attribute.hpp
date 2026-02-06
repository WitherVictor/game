#ifndef ITEM_ATTRIBUTE_HPP
#define ITEM_ATTRIBUTE_HPP

#include <nlohmann/json.hpp>

#include <string>

struct attribute {
    // 返回属性名
    virtual std::string get_name() const = 0;

    // 读取属性值
    virtual void from_json(const nlohmann::json&) = 0;

    // 基类析构函数必须为虚
    virtual ~attribute() = default;
};

#endif // End of macro ITEM_ATTRIBUTE_HPP