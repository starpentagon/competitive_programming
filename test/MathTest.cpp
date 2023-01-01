#include <gtest/gtest.h>
#include "../include/Math.hpp"

TEST(MathTest, TestGCD) {
   EXPECT_EQ(0, GCD(0, 0));
   EXPECT_EQ(1, GCD(1, 0));

   EXPECT_EQ(6, GCD(30, 12));
   EXPECT_EQ(1, GCD(998244353, 1000000007));
}

TEST(MathTest, TestLCM) {
   EXPECT_EQ(48, LCM(12, 16));
   EXPECT_EQ(998244353LL * 1000000007, LCM(998244353, 1000000007));

   {
      // オーバーフロー対策がされているか
      long long a = 998244353LL;
      long long b = 1000000007LL * 998244353LL;
      EXPECT_EQ(998244353LL * 1000000007, LCM(a, b));
   }
}
