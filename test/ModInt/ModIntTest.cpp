#include <gtest/gtest.h>
#include "../../include/ModInt.hpp"

TEST(ModIntStatic, TestAdd) {
   ModIntStatic<11> val(1);

   EXPECT_EQ(1, val);

   val += 9;
   EXPECT_EQ(10, val);

   val++;
   EXPECT_EQ(0, val);
}

TEST(ModIntStatic, TestSub) {
   ModIntStatic<11> val(1);

   val -= 1;
   EXPECT_EQ(0, val);

   val--;
   EXPECT_EQ(10, val);
}

TEST(ModIntStatic, TestMul) {
   ModIntStatic<11> val(1);

   val *= 12;
   EXPECT_EQ(1, val);

   val = 2;
   val = val.pow(10);
   EXPECT_EQ(1024 % 11, val);

   {
      ModIntStatic<11> val(2);
      EXPECT_EQ(val.pow(-1), 6);
   }
}

TEST(ModIntStatic, TestDiv) {
   ModIntStatic<11> val(2);

   auto inv = val.inv();
   EXPECT_EQ(1, val * inv);
}

TEST(ModIntDynamic, TestAdd) {
   ModIntDynamic::SetMod(11);
   ModIntDynamic val(1);

   EXPECT_EQ(1, val);

   val += 9;
   EXPECT_EQ(10, val);

   val++;
   EXPECT_EQ(0, val);
}

TEST(ModIntDynamic, TestSub) {
   ModIntDynamic::SetMod(11);
   ModIntDynamic val(1);

   val -= 1;
   EXPECT_EQ(0, val);

   val--;
   EXPECT_EQ(10, val);
}

TEST(ModIntDynamic, TestMul) {
   ModIntDynamic::SetMod(11);
   ModIntDynamic val(1);

   val *= 12;
   EXPECT_EQ(1, val);

   val = 2;
   val = val.pow(10);
   EXPECT_EQ(1024 % 11, val);
}

TEST(ModIntDynamic, TestDiv) {
   ModIntDynamic::SetMod(11);
   ModIntDynamic val(2);

   auto inv = val.inv();

   EXPECT_EQ(1, val * inv);
   {
      ModIntDynamic val(2);
      EXPECT_EQ(val.pow(-1), 6);
   }
}
