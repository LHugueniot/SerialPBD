#include "gtest/gtest.h"
#include "utilities.h"

TEST(indexDuo, constructor)
{
    int a = 15;
    int b = 10;
    LuHu::indexDuo Test(a, b);

    ASSERT_EQ(Test.p1,b);
    ASSERT_EQ(Test.p2,a);
}
