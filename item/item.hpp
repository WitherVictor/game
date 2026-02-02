#ifndef ITEMS_ITEM_HPP
#define ITEMS_ITEM_HPP

#include <string>
#include <functional>

struct item_info {
    std::string name;
    std::string description;
    std::size_t amount;
};

class item {
public:
    item(const std::string& name, std::size_t amount, std::function<void()> use_effect)
        : name_{name}, amount_{amount}, use_effect_{use_effect} {}

    std::string get_name() const {
        return name_;
    }

    virtual std::string get_description() const {
        return "这是虚拟物品的基类，按理来说你不应该看见这个";
    }

    virtual std::size_t get_amount() const {
        return amount_;
    }

    item_info get_info() const {
        return {
            .name = get_name(),
            .description = get_description(),
            .amount = get_amount()
        };
    }

    bool use() {
        if (amount_ > 0) {
            amount_--;
            use_effect_();
            return true;
        }

        return false;
    }

    void add(std::size_t amount = 1) {
        amount_++;
    }

    void remove(std::size_t amount = 1) {
        amount_--;
    }
protected:
    std::string name_;
    std::function<void()> use_effect_;
    std::size_t amount_;
};

#endif // End of macro ITEMS_ITEM_HPP