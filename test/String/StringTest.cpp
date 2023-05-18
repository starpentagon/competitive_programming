
#include <gtest/gtest.h>
#include "../../include/String.hpp"

TEST(RollingHashTest, CompareTest) {
   RollingHash rolling_hash;

   string S = "hello world.";
   string T = "hello.";

   auto s_hash_list = rolling_hash.build(S);
   auto t_hash_list = rolling_hash.build(T);

   EXPECT_TRUE(rolling_hash.query(s_hash_list, 0, 5) == rolling_hash.query(t_hash_list, 0, 5));
   EXPECT_TRUE(rolling_hash.query(s_hash_list, 0, 6) != rolling_hash.query(t_hash_list, 0, 6));
}

TEST(RollingHashTest, CombineTest) {
   RollingHash rolling_hash;

   string S1 = "hello";
   string S2 = "goodbye world";
   string T = "hello world";

   auto s1_hash_list = rolling_hash.build(S1);
   auto s2_hash_list = rolling_hash.build(S2);
   auto t_hash_list = rolling_hash.build(T);

   auto s_hash = rolling_hash.combine(rolling_hash.query(s1_hash_list, 0, 5), rolling_hash.query(s2_hash_list, 7, 13), 13 - 7);
   auto t_hash = t_hash_list.back();

   EXPECT_TRUE(s_hash == t_hash);
}

TEST(RollingHashTest, LCPTest) {
   RollingHash rolling_hash;

   string S = "hello world.";
   string T = "hello.";

   auto s_hash_list = rolling_hash.build(S);
   auto t_hash_list = rolling_hash.build(T);

   EXPECT_EQ(5, rolling_hash.lcp(s_hash_list, 0, S.size(), t_hash_list, 0, T.size()));
}
