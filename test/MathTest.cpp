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

TEST(MathTest, TestIsPrime) {
   EXPECT_FALSE(IsPrime(1));
   EXPECT_TRUE(IsPrime(2));
   EXPECT_TRUE(IsPrime(3));
   EXPECT_FALSE(IsPrime(4));
   EXPECT_TRUE(IsPrime(5));
   EXPECT_TRUE(IsPrime(998244353LL));
   EXPECT_FALSE(IsPrime(99991LL * 99991));
   EXPECT_TRUE(IsPrime(200560490131LL));
}

TEST(MathTest, TestEnumerateDivisors) {
   {
      auto list = EnumerateDivisors(1);
      ASSERT_EQ(1, list.size());
      ASSERT_EQ(1, list[0]);
   }
   {
      auto list = EnumerateDivisors(12);
      ASSERT_EQ(6, list.size());
      ASSERT_EQ(1, list[0]);
      ASSERT_EQ(2, list[1]);
      ASSERT_EQ(3, list[2]);
      ASSERT_EQ(4, list[3]);
      ASSERT_EQ(6, list[4]);
      ASSERT_EQ(12, list[5]);
   }
   {
      auto list = EnumerateDivisors(99991LL * 99991);
      ASSERT_EQ(3, list.size());
      ASSERT_EQ(1, list[0]);
      ASSERT_EQ(99991LL, list[1]);
      ASSERT_EQ(99991LL * 99991LL, list[2]);
   }
   {
      auto list = EnumerateDivisors(200560490131LL);
      ASSERT_EQ(2, list.size());
      ASSERT_EQ(1, list[0]);
      ASSERT_EQ(200560490131LL, list[1]);
   }
}

TEST(MathTest, TestPower) {
   EXPECT_EQ(1, ipow(1, numeric_limits<long long>::max()));
   EXPECT_EQ(1024, ipow(2, 10));
}

TEST(MathTest, TestCombDP) {
   CombTable comb_tbl(1000);

   EXPECT_EQ(1, comb_tbl.get(10, 0));
   EXPECT_EQ(10, comb_tbl.get(10, 1));
   EXPECT_EQ(1, comb_tbl.get(10, 0));
   EXPECT_EQ(0, comb_tbl.get(10, 11));
   EXPECT_EQ(1, comb_tbl.get(0, 0));
   EXPECT_EQ(0, comb_tbl.get(-1, 0));
   EXPECT_EQ(0, comb_tbl.get(0, -1));
   EXPECT_EQ(1000 * 999 * 998 / 6, comb_tbl.get(1000, 3));
}
