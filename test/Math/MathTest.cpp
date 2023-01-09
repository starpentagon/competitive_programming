#include <gtest/gtest.h>
#include "../../include/Math.hpp"

TEST(MathTest, TestGCD) {
   // 標準ライブラリに置き換え済み
   EXPECT_EQ(0, gcd(0, 0));
   EXPECT_EQ(1, gcd(1, 0));

   EXPECT_EQ(6, gcd(30, 12));
   EXPECT_EQ(1, gcd(998244353, 1000000007));
}

TEST(MathTest, TestLCM) {
   // 標準ライブラリに置き換え済み
   EXPECT_EQ(48, lcm(12, 16));
   EXPECT_EQ(998244353LL * 1000000007, lcm(998244353LL, 1000000007LL));

   {
      // オーバーフロー対策がされているか
      long long a = 998244353LL;
      long long b = 1000000007LL * 998244353LL;
      EXPECT_EQ(998244353LL * 1000000007, lcm(a, b));
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

TEST(MathTest, TestFraction) {
   // 比較演算子
   {
      Fraction a(1, 3), b(1, 2);
      EXPECT_TRUE(a < b);
   }

   // 通分
   {
      Fraction a(12, 16, true);
      EXPECT_EQ(3, a.get_num());
      EXPECT_EQ(4, a.get_denom());
   }

   // 足し算
   {
      Fraction a(1, 3), b(1, 2);
      a += b;
      EXPECT_EQ(Fraction(5, 6), a);
   }
   {
      Fraction a(2, 3), b(5, 9);
      auto c = a + b;
      EXPECT_EQ(Fraction(11, 9), c);
   }

   // 引き算
   {
      Fraction a(1, 3), b(1, 2);
      a -= b;
      EXPECT_EQ(Fraction(-1, 6), a);
   }
   {
      Fraction a(2, 3);
      auto c = a - a;
      EXPECT_EQ(Fraction(0, 3), c);
   }

   // 掛け算
   {
      Fraction a(1, 3), b(1, 2);
      a *= b;
      EXPECT_EQ(Fraction(1, 6), a);
   }
   {
      Fraction a(2, 3), b(4, 7);
      auto c = a * b;
      EXPECT_EQ(Fraction(8, 21), c);
   }

   // 割り算
   {
      Fraction a(1, 3), b(1, 2);
      a /= b;
      EXPECT_EQ(Fraction(2, 3), a);
   }
   {
      Fraction a(2, 3), b(0, 2);
      a /= b;
      EXPECT_EQ(1, a.get_num());
      EXPECT_EQ(0, a.get_denom());
   }
   {
      Fraction a(2, 3), b(4, 7);
      auto c = a / b;
      EXPECT_EQ(Fraction(7, 6), c);
   }
}

TEST(MathTest, TestExponentOfN) {
   EXPECT_EQ(10, CalcExponentOfN(2, 1024));
   EXPECT_EQ(0, CalcExponentOfN(3, 1024));
}

TEST(MathTest, TestExponentOfFactorial) {
   EXPECT_EQ(3, CalcExponentOfFactorial(2, 5));
   EXPECT_EQ(1, CalcExponentOfFactorial(3, 5));
   EXPECT_EQ(1, CalcExponentOfFactorial(5, 5));
   EXPECT_EQ(0, CalcExponentOfFactorial(7, 5));
}

TEST(MathTest, TestPrimeFactorization) {
   using P = pair<long long, long long>;

   {
      int N = 2;
      auto list = PrimeFactorization(N);

      ASSERT_EQ(1, list.size());
      EXPECT_EQ(P(2, 1), list[0]);
   }
   {
      int N = 68;
      auto list = PrimeFactorization(N);

      ASSERT_EQ(2, list.size());
      EXPECT_EQ(P(2, 2), list[0]);
      EXPECT_EQ(P(17, 1), list[1]);
   }
}

TEST(MathTest, TestPrimeTableIsPrime) {
   PrimeTable prime_table;
   prime_table.Build(1000, false);

   ASSERT_TRUE(prime_table.IsPrime(2));
   ASSERT_TRUE(prime_table.IsPrime(3));
   ASSERT_FALSE(prime_table.IsPrime(4));
}

TEST(MathTest, TestPrimeTablePrimeFactor) {
   PrimeTable prime_table;
   prime_table.Build(1000, true);

   using P = pair<long long, long long>;

   {
      int N = 1;
      auto list = prime_table.PrimeFactorization(N);

      ASSERT_EQ(1, list.size());
      ASSERT_EQ(P(2, 0), list[0]);
   }
   {
      int N = 10;
      auto list = prime_table.PrimeFactorization(N);

      ASSERT_EQ(2, list.size());
      ASSERT_EQ(P(2, 1), list[0]);
      ASSERT_EQ(P(5, 1), list[1]);
   }
   {
      int N = 360;  // 360 = 2^3 * 3^2 * 5
      auto list = prime_table.PrimeFactorization(N);

      ASSERT_EQ(3, list.size());
      ASSERT_EQ(P(2, 3), list[0]);
      ASSERT_EQ(P(3, 2), list[1]);
      ASSERT_EQ(P(5, 1), list[2]);
   }
   {
      int N = 991;
      auto list = prime_table.PrimeFactorization(N);

      ASSERT_EQ(1, list.size());
      ASSERT_EQ(P(991, 1), list[0]);
   }
}

TEST(MathTest, TestPrimeTableIntervalIsPrime) {
   PrimeTable prime_table;
   prime_table.IntervalBuild(10000, 1000, false);  // [9001, 10000]

   ASSERT_TRUE(prime_table.IsIntervalPrime(9001));
   ASSERT_FALSE(prime_table.IsIntervalPrime(9003));
   ASSERT_FALSE(prime_table.IsIntervalPrime(9999));
   ASSERT_FALSE(prime_table.IsIntervalPrime(10000));
}

TEST(MathTest, TestPrimeTableIntervalFactor) {
   using P = pair<long long, long long>;
   PrimeTable prime_table;
   prime_table.IntervalBuild(10000, 1000, true);  // [9001, 10000]

   {
      int N = 9001;
      auto list = prime_table.IntervalPrimeFactorization(N);

      ASSERT_EQ(1, list.size());
      ASSERT_EQ(P(9001, 1), list[0]);
   }
   {
      int N = 9999;  // 9999 = 3^2 * 11 * 101
      auto list = prime_table.IntervalPrimeFactorization(N);

      ASSERT_EQ(3, list.size());
      ASSERT_EQ(P(3, 2), list[0]);
      ASSERT_EQ(P(11, 1), list[1]);
      ASSERT_EQ(P(101, 1), list[2]);
   }
}

TEST(MathTest, TestSqrtInt) {
   using P = pair<long long, long long>;

   {
      long long n = 0;
      ASSERT_EQ(P(0, 0), sqrt_int(n));
   }
   {
      long long n = 1;
      ASSERT_EQ(P(1, 1), sqrt_int(n));
   }
   {
      long long n = 2;
      ASSERT_EQ(P(1, 2), sqrt_int(n));
   }
   {
      long long n = numeric_limits<long long>::max();
      ASSERT_EQ(P(3037000499, 3037000500), sqrt_int(n));
   }
}