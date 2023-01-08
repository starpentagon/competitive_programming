#include <gtest/gtest.h>
#include <limits>
// #include "../../include/Search.hpp"

using namespace std;
using ll = long long;

ll calc_mid(ll ok, ll ng) {
   long long m = ok / 2 + ng / 2;
   m += ((ok % 2) + (ng % 2)) / 2;

   return m;
}

TEST(SearchTest, TestCalcMid) {
   EXPECT_EQ(1, calc_mid(0, 2));

   ll kMax = numeric_limits<ll>::max();
   EXPECT_EQ(kMax - 1, calc_mid(kMax, kMax - 2));

   EXPECT_EQ(-3, calc_mid(-2, -5));
   EXPECT_EQ(-4, calc_mid(-2, -6));
   EXPECT_EQ(-4, calc_mid(-3, -5));

   EXPECT_EQ(0, calc_mid(-2, 3));
   EXPECT_EQ(0, calc_mid(-2, 2));
   EXPECT_EQ(0, calc_mid(-3, 3));

   ll kMin = numeric_limits<ll>::min();
   EXPECT_EQ(kMin + 1, calc_mid(kMin, kMin + 2));
}
