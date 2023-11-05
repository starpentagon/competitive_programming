#include <gtest/gtest.h>
#include "../../../include/Misc.hpp"

TEST(MathTest, TestBaseConversion) {
   BaseConversion base(2, "1011");

   EXPECT_EQ(11, base.GetBase10Val());
   EXPECT_EQ("102", base.GetBaseStr(3));
}
