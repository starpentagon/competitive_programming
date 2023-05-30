#include <gtest/gtest.h>
#include "../../include/ExtInt.hpp"

TEST(ExtIntTest, TestFiniteAdd) {
   // -ninf < 有限 + 有限 < inf
   {
      ExtInt val;
      EXPECT_EQ(0, val);

      val += 1;
      EXPECT_EQ(1, val);
   }
   {
      ExtInt val(eint::INF - 2);
      val += 1;
      EXPECT_EQ(eint::INF - 1, val);
   }
   {
      ExtInt val(eint::NINF + 1);
      val += 1;
      EXPECT_EQ(eint::NINF + 2, val);
   }
}

TEST(ExtIntTest, TestFiniteInfiniteAdd) {
   // 有限 + inf
   {
      ExtInt val(1);
      val += eint::INF;
      EXPECT_EQ(eint::INF, val);
   }

   // inf + 有限
   {
      ExtInt val(eint::INF);
      val += 1;
      EXPECT_EQ(eint::INF, val);
   }

   // 有限 + ninf
   {
      ExtInt val(1);
      val += eint::NINF;
      EXPECT_EQ(eint::NINF, val);
   }

   // ninf + 有限
   {
      ExtInt val(eint::NINF);
      val += 1;
      EXPECT_EQ(eint::NINF, val);
   }
}

TEST(ExtIntTest, TestInfiniteInfiniteAdd) {
   // inf + inf
   {
      ExtInt val(eint::INF);
      val += eint::INF;
      EXPECT_EQ(eint::INF, val);
   }
   // inf + ninf(death test)
   {
      ExtInt val(eint::INF);
      ASSERT_DEATH(val += eint::NINF, ".* failed.");
   }
   // ninf + inf(death test)
   {
      ExtInt val(eint::NINF);
      ASSERT_DEATH(val += eint::INF, ".* failed.");
   }
   // ninf + ninf
   {
      ExtInt val(eint::NINF);
      val += eint::NINF;
      EXPECT_EQ(eint::NINF, val);
   }
}

TEST(ExtIntTest, TestMinus) {
   // 有限
   {
      ExtInt val(1);
      val = -val;
      EXPECT_EQ(-1, val);
   }

   // inf
   {
      ExtInt val(eint::INF);
      val = -val;
      EXPECT_EQ(eint::NINF, val);
   }

   // ninf
   {
      ExtInt val(eint::NINF);
      val = -val;
      EXPECT_EQ(eint::INF, val);
   }

   // ninf + 1(finite, but -(ninf + 1) = inf)
   {
      ExtInt val(eint::NINF + 1);
      val = -val;
      EXPECT_EQ(eint::INF, val);
   }
}

TEST(ExtIntTest, TestIncrement) {
   // 有限 -> 有限
   {
      ExtInt val(1);
      val++;
      EXPECT_EQ(2, val);
   }

   // 有限 -> inf
   {
      ExtInt val(eint::INF - 1);
      val++;
      EXPECT_EQ(eint::INF, val);
   }

   // inf -> inf
   {
      ExtInt val(eint::INF);
      val++;
      EXPECT_EQ(eint::INF, val);
   }

   // ninf -> ninf
   {
      ExtInt val(eint::NINF);
      val++;
      EXPECT_EQ(eint::NINF, val);
   }
}

TEST(ExtIntTest, TestDecrement) {
   // 有限 -> 有限
   {
      ExtInt val(1);
      val--;
      EXPECT_EQ(0, val);
   }

   // 有限 -> ninf
   {
      ExtInt val(eint::NINF + 1);
      val--;
      EXPECT_EQ(eint::NINF, val);
   }

   // inf -> inf
   {
      ExtInt val(eint::INF);
      val--;
      EXPECT_EQ(eint::INF, val);
   }

   // ninf -> ninf
   {
      ExtInt val(eint::NINF);
      val--;
      EXPECT_EQ(eint::NINF, val);
   }
}

TEST(ExtIntTest, TestMulFiniteFinte) {
   // -ninf < 有限 * 有限 < inf
   {
      ExtInt val(2);
      val *= 3;
      EXPECT_EQ(6, val);
   }
   {
      ExtInt val(2 * 715827883LL);
      val *= 3 * 2147483647LL;
      EXPECT_EQ(eint::INF - 1, val);
   }
   {
      ExtInt val(-7 * 7 * 73 * 127 * 337);
      val *= 92737 * 649657LL;
      EXPECT_EQ(eint::NINF + 1, val);
   }
}

TEST(ExtIntTest, TestMulZeroFinte) {
   // 0 * 有限
   {
      ExtInt val(eint::INF - 1);
      val *= 0;
      EXPECT_EQ(0, val);
   }
   {
      ExtInt val(0);
      val *= eint::NINF + 1;
      EXPECT_EQ(0, val);
   }
}

TEST(ExtIntTest, TestMulZeroInfinte) {
   // 0 * inf(death test)
   {
      ExtInt val(eint::INF);
      ASSERT_DEATH(val *= 0, ".* failed.");
   }
   {
      ExtInt val(0);
      ASSERT_DEATH(val *= eint::INF, ".* failed.");
   }

   // 0 * ninf(death test)
   {
      ExtInt val(eint::NINF);
      ASSERT_DEATH(val *= 0, ".* failed.");
   }
   {
      ExtInt val(0);
      ASSERT_DEATH(val *= eint::NINF, ".* failed.");
   }
}

TEST(ExtIntTest, TestMulFiniteToInfinite) {
   // 有限 * 有限 -> inf
   {
      ExtInt val(7 * 7 * 73 * 127 * 337);
      val *= 92737 * 649657LL;
      EXPECT_EQ(eint::INF, val);
   }
   {
      ExtInt val(1LL << 32);
      val *= 1LL << 32;
      EXPECT_EQ(eint::INF, val);
   }

   // 有限 * 有限 -> ninf
   {
      ExtInt val(-2 * 2 * 2);
      val *= 1152921504606846976;
      EXPECT_EQ(eint::NINF, val);
   }
   {
      ExtInt val(1LL << 32);
      val *= -(1LL << 32);
      EXPECT_EQ(eint::NINF, val);
   }
}

TEST(ExtIntTest, TestMulFiniteInfinite) {
   // 有限 * inf -> inf
   {
      ExtInt val(1);
      val *= eint::INF;
      EXPECT_EQ(eint::INF, val);
   }
   {
      ExtInt val(eint::INF);
      val *= 2;
      EXPECT_EQ(eint::INF, val);
   }
   {
      ExtInt val(eint::INF);
      val *= -1;
      EXPECT_EQ(eint::NINF, val);
   }
   {
      ExtInt val(-2);
      val *= eint::INF;
      EXPECT_EQ(eint::NINF, val);
   }

   // 有限 * ninf -> ninf
   {
      ExtInt val(1);
      val *= eint::NINF;
      EXPECT_EQ(eint::NINF, val);
   }
   {
      ExtInt val(eint::NINF);
      val *= 2;
      EXPECT_EQ(eint::NINF, val);
   }
   {
      ExtInt val(-1);
      val *= eint::NINF;
      EXPECT_EQ(eint::INF, val);
   }
}

TEST(ExtIntTest, TestMulInfiniteInfinite) {
   // inf * inf -> inf
   {
      ExtInt val(eint::INF);
      val *= eint::INF;
      EXPECT_EQ(eint::INF, val);
   }

   // ninf * inf -> ninf
   {
      ExtInt val(eint::INF);
      val *= eint::NINF;
      EXPECT_EQ(eint::NINF, val);
   }
   {
      ExtInt val(eint::NINF);
      val *= eint::INF;
      EXPECT_EQ(eint::NINF, val);
   }

   // ninf * ninf -> ninf
   {
      ExtInt val(eint::NINF);
      val *= eint::NINF;
      EXPECT_EQ(eint::INF, val);
   }
}

TEST(ExtIntTest, TestDivFiniteFinite) {
   // 有限 / 有限
   {
      ExtInt val(8);
      val /= 2;
      EXPECT_EQ(4, val);
   }
   {
      ExtInt val(5);
      val /= -2;
      EXPECT_EQ(-2, val);
   }
   {
      ExtInt val(-10);
      val /= -3;
      EXPECT_EQ(3, val);
   }
}

TEST(ExtIntTest, TestDivInfiniteFinite) {
   // 無限 / 有限(非ゼロ)
   {
      ExtInt val(eint::INF);
      val /= 2;
      EXPECT_EQ(eint::INF, val);
   }
   {
      ExtInt val(eint::INF);
      val /= -2;
      EXPECT_EQ(eint::NINF, val);
   }
   {
      ExtInt val(eint::NINF);
      val /= 2;
      EXPECT_EQ(eint::NINF, val);
   }
   {
      ExtInt val(eint::NINF);
      val /= -2;
      EXPECT_EQ(eint::INF, val);
   }
}

TEST(ExtIntTest, TestDivInfiniteZero) {
   // 無限 / 有限(ゼロ), death test
   {
      ExtInt val(eint::INF);
      ASSERT_DEATH(val /= 0, ".* failed.");
   }
   {
      ExtInt val(eint::NINF);
      ASSERT_DEATH(val /= 0, ".* failed.");
   }
}

TEST(ExtIntTest, TestDivFiniteInfinite) {
   // 有限 / 無限
   {
      ExtInt val(1);
      val /= eint::INF;
      EXPECT_EQ(0, val);
   }
   {
      ExtInt val(0);
      val /= eint::INF;
      EXPECT_EQ(0, val);
   }
   {
      ExtInt val(eint::NINF + 1);
      val /= eint::INF;
      EXPECT_EQ(0, val);
   }
   {
      ExtInt val(eint::INF - 1);
      val /= eint::NINF;
      EXPECT_EQ(0, val);
   }
   {
      ExtInt val(0);
      val /= eint::NINF;
      EXPECT_EQ(0, val);
   }
   {
      ExtInt val(eint::NINF + 1);
      val /= eint::NINF;
      EXPECT_EQ(0, val);
   }
}

TEST(ExtIntTest, TestDivInfiniteInfinite) {
   // 無限 / 無限, death test
   {
      ExtInt val(eint::INF);
      ASSERT_DEATH(val /= eint::INF, ".* failed.");
   }
   {
      ExtInt val(eint::INF);
      ASSERT_DEATH(val /= eint::NINF, ".* failed.");
   }
   {
      ExtInt val(eint::NINF);
      ASSERT_DEATH(val /= eint::INF, ".* failed.");
   }
   {
      ExtInt val(eint::NINF);
      ASSERT_DEATH(val /= eint::NINF, ".* failed.");
   }
}

TEST(ExtIntTest, TestCompare) {
   // 有限 < 有限
   {
      ExtInt val1(1), val2(2);
      EXPECT_TRUE(val1 < val2);
   }
   {
      ExtInt val1(1), val2(1);
      EXPECT_FALSE(val1 < val2);
   }

   // ninf < 有限
   {
      ExtInt val1(eint::NINF), val2(eint::NINF + 1);
      EXPECT_TRUE(val1 < val2);
   }

   // 有限 < inf
   {
      ExtInt val1(1), val2(eint::INF);
      EXPECT_TRUE(val1 < val2);
   }

   // ninf < inf
   {
      ExtInt val1(eint::NINF), val2(eint::INF);
      EXPECT_TRUE(val1 < val2);
   }
   {
      ExtInt val1(eint::INF), val2(eint::INF);
      EXPECT_FALSE(val1 < val2);
   }
   {
      ExtInt val1(eint::NINF), val2(eint::NINF);
      EXPECT_FALSE(val1 < val2);
   }
}

TEST(ExtIntTest, TestModFiniteFinite) {
   // 有限 % 有限
   {
      ExtInt val(8);
      val %= 2;
      EXPECT_EQ(0, val);
   }
   {
      ExtInt val(5);
      val %= -2;
      EXPECT_EQ(1, val);
   }
   {
      ExtInt val(-10);
      val %= -3;
      EXPECT_EQ(-1, val);
   }

   // 有限 % 0, death test
   {
      ExtInt val(3);
      ASSERT_DEATH(val %= 0, ".* failed.");
   }
}

TEST(ExtIntTest, TestModInfiniteFinite) {
   // 無限 % 有限(非ゼロ)
   {
      ExtInt val(eint::INF);
      ASSERT_DEATH(val %= 1, ".* failed.");
   }
   {
      ExtInt val(eint::NINF);
      ASSERT_DEATH(val %= 2, ".* failed.");
   }
}

TEST(ExtIntTest, TestModInfiniteZero) {
   // 無限 % 有限(ゼロ), death test
   {
      ExtInt val(eint::INF);
      ASSERT_DEATH(val %= 0, ".* failed.");
   }
   {
      ExtInt val(eint::NINF);
      ASSERT_DEATH(val %= 0, ".* failed.");
   }
}

TEST(ExtIntTest, TestModFiniteInfinite) {
   // 有限 / 無限, death test
   {
      ExtInt val(1);
      ASSERT_DEATH(val %= eint::INF, ".* failed.");
   }
   {
      ExtInt val(0);
      ASSERT_DEATH(val %= eint::NINF, ".* failed.");
   }
}

TEST(ExtIntTest, TestModInfiniteInfinite) {
   // 無限 / 無限, death test
   {
      ExtInt val(eint::INF);
      ASSERT_DEATH(val %= eint::INF, ".* failed.");
   }
   {
      ExtInt val(eint::INF);
      ASSERT_DEATH(val %= eint::NINF, ".* failed.");
   }
   {
      ExtInt val(eint::NINF);
      ASSERT_DEATH(val %= eint::INF, ".* failed.");
   }
   {
      ExtInt val(eint::NINF);
      ASSERT_DEATH(val %= eint::NINF, ".* failed.");
   }
}
