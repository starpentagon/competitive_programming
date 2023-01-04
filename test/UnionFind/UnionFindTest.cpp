#include <gtest/gtest.h>
#include "../../include/UnionFind.hpp"

TEST(UnionTest, TestSame) {
   UnionFind uf(10);

   EXPECT_FALSE(uf.IsSameGroup(1, 2));

   uf.Unite(1, 2);
   EXPECT_TRUE(uf.IsSameGroup(1, 2));
}

TEST(UnionTest, TestRoot) {
   UnionFind uf(10);

   uf.Unite(1, 2);
   auto p1 = uf.root(1);
   auto p2 = uf.root(2);

   ASSERT_EQ(p1, p2);
}

TEST(UnionTest, TestCCGroup) {
   UnionFind uf(10);

   uf.Unite(1, 2);

   auto cc_group = uf.EnumGroup();

   for (auto [p, vec] : cc_group) {
      if (vec.size() == 2) {
         EXPECT_TRUE(find(vec.begin(), vec.end(), 1) != vec.end());
         EXPECT_TRUE(find(vec.begin(), vec.end(), 2) != vec.end());
      }
   }
}
