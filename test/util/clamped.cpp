#include <gtest/gtest.h>

#include "util/clamped.hpp"

TEST(clamped, common) {
    clamped value{0, 50, 100};

    EXPECT_TRUE(value.try_add(20));
    EXPECT_EQ(value.get_now(), 70);

    EXPECT_TRUE(value.try_minus(65));
    EXPECT_EQ(value.get_now(), 5);
}

TEST(clamped, try_bound) {
    clamped value{0, 50, 100};

    EXPECT_FALSE(value.try_add(55));
    EXPECT_EQ(value.get_now(), 50);

    EXPECT_FALSE(value.try_minus(55));
    EXPECT_EQ(value.get_now(), 50);
}

TEST(clamped, force_bound) {
    clamped value{0, 50, 100};

    value.force_add(55);
    EXPECT_EQ(value.get_now(), 100);

    value.force_minus(155);
    EXPECT_EQ(value.get_now(), 0);
}