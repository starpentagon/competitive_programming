#include <gtest/gtest.h>
#include "../../include/SegmentTree.hpp"

TEST(SegmentTreeTest, find_leq) {
   vector<int> val_list = {9, 9, 8, 7, 8, 2, 0, 2, 3, 1};
   int N = val_list.size();

   {
      int unit_x = numeric_limits<int>::max();

      SegmentTree<int> segtree(
          val_list, [](int a, int b) { return min(a, b); }, unit_x);

      ASSERT_EQ(segtree.find_leftest_leq(0, N, 0), 6);
      ASSERT_EQ(segtree.find_leftest_leq(0, N, 1), 6);
      ASSERT_EQ(segtree.find_leftest_leq(7, N, 1), 9);

      ASSERT_EQ(segtree.find_leftest_leq(0, N, 2), 5);
      ASSERT_EQ(segtree.find_leftest_leq(0, N, 9), 0);
      ASSERT_EQ(segtree.find_leftest_leq(1, N, 9), 1);
      ASSERT_EQ(segtree.find_leftest_leq(1, N, -2), -1);
   }
   {
      int unit_x = numeric_limits<int>::max();

      SegmentTree<int> segtree(
          val_list, [](int a, int b) { return min(a, b); }, unit_x);

      ASSERT_EQ(segtree.find_rightest_leq(0, N, 0), 6);
      ASSERT_EQ(segtree.find_rightest_leq(0, N, 1), 9);
      ASSERT_EQ(segtree.find_rightest_leq(0, N - 1, 1), 6);

      ASSERT_EQ(segtree.find_rightest_leq(0, 7, 2), 6);
      ASSERT_EQ(segtree.find_rightest_leq(0, N, 9), 9);
      ASSERT_EQ(segtree.find_rightest_leq(0, 1, 9), 0);
      ASSERT_EQ(segtree.find_rightest_leq(1, N, -2), -1);
   }
}

TEST(SegmentTreeTest, find_geq) {
   vector<int> val_list = {0, 2, 3, 1, 9, 9, 8, 7, 8, 2};
   int N = val_list.size();
   int unit_x = numeric_limits<int>::min();

   auto oper = [](int a, int b) { return max(a, b); };

   {
      SegmentTree<int> segtree(val_list, oper, unit_x);

      ASSERT_EQ(segtree.find_leftest_geq(0, N, 0), 0);
      ASSERT_EQ(segtree.find_leftest_geq(0, N, 1), 1);
      ASSERT_EQ(segtree.find_leftest_geq(7, N, 3), 7);

      ASSERT_EQ(segtree.find_leftest_geq(0, N, 4), 4);
      ASSERT_EQ(segtree.find_leftest_geq(0, N, 9), 4);
      ASSERT_EQ(segtree.find_leftest_geq(6, N, 9), -1);
   }
   {
      SegmentTree<int> segtree(val_list, oper, unit_x);

      ASSERT_EQ(segtree.find_rightest_geq(0, N, 0), 9);
      ASSERT_EQ(segtree.find_rightest_geq(0, N, 3), 8);
      ASSERT_EQ(segtree.find_rightest_geq(0, N - 1, 2), 8);

      ASSERT_EQ(segtree.find_rightest_geq(0, 7, 2), 6);
      ASSERT_EQ(segtree.find_rightest_geq(0, N, 9), 5);
      ASSERT_EQ(segtree.find_leftest_geq(0, N, 10), -1);
   }
}

TEST(SegmentTreeTest, find_sum_geq) {
   vector<int> val_list = {9, 9, 8, 7, 8, 2, 0, 2, 3, 1};
   int N = val_list.size();
   int unit_x = 0;

   auto oper = [](int a, int b) { return a + b; };

   {
      SegmentTree<int> segtree(val_list, oper, unit_x);

      ASSERT_EQ(segtree.find_leftest_sum_geq(0, N, 18), 1);
      ASSERT_EQ(segtree.find_leftest_sum_geq(0, N, 33), 3);
      ASSERT_EQ(segtree.find_leftest_sum_geq(0, N, 49), 9);
      ASSERT_EQ(segtree.find_leftest_sum_geq(0, N, 50), -1);

      ASSERT_EQ(segtree.find_leftest_sum_geq(4, N, 10), 5);
      ASSERT_EQ(segtree.find_leftest_sum_geq(4, N, 11), 7);
   }
}
