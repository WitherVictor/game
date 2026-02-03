#ifndef ITEMS_ITEM_HPP
#define ITEMS_ITEM_HPP

#include <string>
#include <functional>

struct item_info {
    std::string name;
    std::string description;
    std::size_t amount;
};

// 代表一个物品
// 拥有名称，说明和数量
class item {
public:
    item(const std::string& name, const std::size_t amount, const std::string& description)
        : name_{name}, amount_{amount}, description_{description} {}

    const std::string& get_name() const {
        return name_;
    }

    const std::size_t get_amount() const {
        return amount_;
    }

    const std::string& get_description() const {
        return description_;
    }

    item_info get_info() const {
        return {
            .name = get_name(),
            .description = get_description(),
            .amount = get_amount()
        };
    }

    void add(std::size_t amount = 1) {
        amount_++;
    }

    void minus(std::size_t amount = 1) {
        amount_--;
    }

    virtual bool use() { return false; }

    virtual ~item() = default;
protected:
    std::string name_;
    std::size_t amount_;
    std::string description_;
};

// 可使用物品
// 使用后会产生某个效果
class usable_item : public item {
public:
    usable_item(const std::string& name, const std::size_t amount, const std::string& description, std::function<void()> use_effect)
        : item{name, amount, description}, use_effect_{use_effect} {}

    virtual bool use() override {
        if (amount_ > 0) {
            minus();
            use_effect_();
            return true;
        }

        return false;
    }
protected:
    std::function<void()> use_effect_;
};

#endif // End of macro ITEMS_ITEM_HPP