
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

TEST(SmartStringTest, ConstructorTest) {
   {
      SmartString str;
      EXPECT_EQ(str, "");
   }
   {
      SmartString str("hello world!");
      EXPECT_EQ(str, "hello world!");
   }
}

TEST(SmartStringTest, ContainsTest) {
   {
      SmartString str("hello world!");
      EXPECT_TRUE(str.Contains("world"));
      EXPECT_FALSE(str.Contains("good"));
   }
}

TEST(SmartStringTest, ContainsRegexTest) {
   {
      SmartString str("AAC");
      EXPECT_TRUE(str.ContainsRegex("A*B*C*"));
      EXPECT_FALSE(str.Contains("A*B+C*"));
   }
}

TEST(SmartStringTest, GetReplaceTest) {
   {
      SmartString str("hello world!");
      auto rep_str = str.GetReplace("hello", "good morning");
      EXPECT_EQ(rep_str, "good morning world!");
   }
   {
      SmartString str("ABC");
      auto rep_str = str.GetReplace("a", "b");
      EXPECT_EQ(rep_str, "ABC");
   }
   {
      SmartString str("ABC");
      auto rep_str = str.GetReplace("A", "B");
      EXPECT_EQ(rep_str, "BBC");
   }
}

TEST(SmartStringTest, ReplaceTest) {
   {
      SmartString str("hello world!");
      str.Replace("hello", "good morning");
      EXPECT_EQ(str, "good morning world!");
   }
   {
      SmartString str("ABC");
      str.Replace("a", "b");
      EXPECT_EQ(str, "ABC");
   }
   {
      SmartString str("ABC");
      str.Replace("A", "B");
      EXPECT_EQ(str, "BBC");
   }
}

TEST(SmartStringTest, IsSubsequenceTest) {
   {
      SmartString str("hello world!");
      ASSERT_TRUE(str.IsSubsequence("hello"));
      ASSERT_TRUE(str.IsSubsequence("hw!"));
      ASSERT_FALSE(str.IsSubsequence("hw!d"));
   }
   {
      SmartString str("abc");
      ASSERT_TRUE(str.IsSubsequence("abc"));
      ASSERT_FALSE(str.IsSubsequence("abcd"));
   }
}

TEST(SmartStringTest, SplitTest) {
   {
      SmartString str("hello world!");
      auto result = str.Split(" ");
      ASSERT_EQ(result.size(), 2);
      ASSERT_EQ(result[0], "hello");
      ASSERT_EQ(result[1], "world!");
   }
   {
      SmartString str("aaababcc");
      auto result = str.Split("abc");
      ASSERT_EQ(result.size(), 2);
      ASSERT_EQ(result[0], "aaab");
      ASSERT_EQ(result[1], "c");
   }
   {
      SmartString str("a,b,c,");
      auto result = str.Split(",");
      ASSERT_EQ(result.size(), 4);
      ASSERT_EQ(result[0], "a");
      ASSERT_EQ(result[1], "b");
      ASSERT_EQ(result[2], "c");
      ASSERT_EQ(result[3], "");
   }
}
