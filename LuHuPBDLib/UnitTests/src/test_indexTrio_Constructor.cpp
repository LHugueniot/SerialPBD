#include "gtest/gtest.h"
#include "utilities.h"

TEST(indexTrio, constructor)
{
    int a = 10;
    int b = 15;
    int c = 30;
    LuHu::indexTrio Test(a, b, c);

    ASSERT_EQ(Test.p1,a);
    ASSERT_EQ(Test.p2,b);
    ASSERT_EQ(Test.p3,c);
}
