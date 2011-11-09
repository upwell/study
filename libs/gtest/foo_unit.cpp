#include "gtest/gtest.h"

#include "foo.h"

TEST(FOO, Valid) {
    EXPECT_TRUE(is_even_number(10));
    EXPECT_TRUE(is_even_number(0));
    EXPECT_FALSE(is_even_number(1));
    EXPECT_FALSE(is_even_number(-1));
}
