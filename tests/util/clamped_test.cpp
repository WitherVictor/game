#include <gtest/gtest.h>

#include <cstddef>
#include <stdexcept>

#include "util/clamped.hpp"

// 构造对象后当前值应为最小值
TEST(clamped_test, default_min_now_test) {
    clamped<std::size_t> obj{0, 100};
    EXPECT_EQ(obj.now(), obj.min());
}

// 最小值大于最大值时应抛出异常
TEST(clamped_test, exception_test) {
    EXPECT_THROW({
        clamped<std::size_t> obj(100, 0);
    }, std::invalid_argument);
}

// 测试 set_*** 函数功能
TEST(clamped_test, set_function_test) {
    clamped<std::size_t> obj{0, 100};

    EXPECT_TRUE(obj.set_now(42));
    EXPECT_FALSE(obj.set_now(500));

    obj.set_min(25);
    EXPECT_TRUE(obj.set_now(35));
    EXPECT_FALSE(obj.set_now(0));
}

// 测试访问值是否正常
TEST(clamped_test, get_function_test) {
    clamped<std::size_t> obj{0, 100};
    obj.set_now(55);

    EXPECT_EQ(obj.min(), 0);
    EXPECT_EQ(obj.now(), 55);
    EXPECT_EQ(obj.max(), 100);
}

// 测试 try_*** 函数功能
TEST(clamped_test, try_function_test) {
    clamped<std::size_t> obj{0, 100};

    EXPECT_FALSE(obj.try_minus());
    EXPECT_TRUE(obj.try_add());

    obj.set_now(100);
    EXPECT_FALSE(obj.try_add());
    EXPECT_TRUE(obj.try_minus());
}

// 测试 force_*** 函数功能
TEST(clamped_test, force_function_test) {
    clamped<std::size_t> obj{0, 100};

    obj.force_add(4242);
    EXPECT_EQ(obj.now(), 100);

    obj.force_minus(4242);
    EXPECT_EQ(obj.now(), 0);
}