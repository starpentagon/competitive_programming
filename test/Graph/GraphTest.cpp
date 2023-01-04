#include <gtest/gtest.h>
#include "../../include/Graph.hpp"

TEST(GraphTest, TestFindLoopUndirected) {
   using Edge = pair<int, int>;

   {
      // 単一連結成分, loopなし
      // 1-2-3-4
      int N = 4;
      vector<vector<int>> adj_list(N + 1);
      vector<Edge> edge_list{Edge(1, 2), Edge(2, 3), Edge(3, 4)};

      for (auto [u, v] : edge_list) {
         adj_list[u].emplace_back(v);
         adj_list[v].emplace_back(u);
      }

      auto loop_group = FindLoopUndirected(N, edge_list);
      ASSERT_TRUE(loop_group.empty());
   }
   {
      // 単一連結成分, loopあり
      // 1-2-3-1, 3-4
      int N = 4;
      vector<vector<int>> adj_list(N + 1);
      vector<Edge> edge_list{Edge(1, 2), Edge(2, 3), Edge(3, 4), Edge(3, 1)};

      for (auto [u, v] : edge_list) {
         adj_list[u].emplace_back(v);
         adj_list[v].emplace_back(u);
      }

      auto loop_group = FindLoopUndirected(N, edge_list);
      ASSERT_EQ(1, loop_group.size());

      auto [start, edge_index_list] = loop_group[0];
      ASSERT_EQ(3, start);
      ASSERT_EQ(1, edge_index_list[0]);
      ASSERT_EQ(0, edge_index_list[1]);
      ASSERT_EQ(3, edge_index_list[2]);
   }
   {
      // 3連結成分, loop2つ
      // 1-2-3-1, 4-5-6-7-4, 8-9
      int N = 9;
      vector<vector<int>> adj_list(N + 1);
      vector<Edge> edge_list{
          Edge(1, 2), Edge(2, 3), Edge(3, 1),
          Edge(4, 5), Edge(5, 6), Edge(6, 7), Edge(7, 4),
          Edge(8, 9)};

      for (auto [u, v] : edge_list) {
         adj_list[u].emplace_back(v);
         adj_list[v].emplace_back(u);
      }

      auto loop_group = FindLoopUndirected(N, edge_list);
      ASSERT_EQ(2, loop_group.size());
      ASSERT_EQ(3, loop_group[0].second.size());
      ASSERT_EQ(4, loop_group[1].second.size());
   }
}

TEST(GraphTest, TestFindLoopDirected) {
   using Edge = pair<int, int>;

   {
      // 単一連結成分, loopなし
      // 1-2-3-4
      int N = 4;
      vector<vector<int>> adj_list(N + 1);
      vector<Edge> edge_list{Edge(1, 2), Edge(2, 3), Edge(3, 4)};

      for (auto [u, v] : edge_list) {
         adj_list[u].emplace_back(v);
      }

      auto loop_group = FindLoopDirected(N, edge_list);
      ASSERT_TRUE(loop_group.empty());
   }
   {
      // 単一連結成分, loopあり
      // 1-2-3-1, 3-4
      int N = 4;
      vector<vector<int>> adj_list(N + 1);
      vector<Edge> edge_list{Edge(1, 2), Edge(2, 3), Edge(3, 4), Edge(3, 1)};

      for (auto [u, v] : edge_list) {
         adj_list[u].emplace_back(v);
      }

      auto loop_group = FindLoopDirected(N, edge_list);
      ASSERT_EQ(1, loop_group.size());

      auto edge_index_list = loop_group[0];
      ASSERT_EQ(0, edge_index_list[0]);
      ASSERT_EQ(1, edge_index_list[1]);
      ASSERT_EQ(3, edge_index_list[2]);
   }
   {
      // 3連結成分, loop2つ
      // 1-2-3-1, 4-5-6-7-4, 8-9
      int N = 9;
      vector<vector<int>> adj_list(N + 1);
      vector<Edge> edge_list{
          Edge(1, 2), Edge(2, 3), Edge(3, 1),
          Edge(4, 5), Edge(5, 6), Edge(6, 7), Edge(7, 4),
          Edge(8, 9)};

      for (auto [u, v] : edge_list) {
         adj_list[u].emplace_back(v);
      }

      auto loop_group = FindLoopDirected(N, edge_list);
      ASSERT_EQ(2, loop_group.size());
      ASSERT_EQ(3, loop_group[0].size());
      ASSERT_EQ(4, loop_group[1].size());
   }
   {
      // 自己ループ
      // 1-2-2
      int N = 2;
      vector<vector<int>> adj_list(N + 1);
      vector<Edge> edge_list{
          Edge(1, 2), Edge(2, 2)};

      for (auto [u, v] : edge_list) {
         adj_list[u].emplace_back(v);
      }

      auto loop_group = FindLoopDirected(N, edge_list);
      ASSERT_EQ(1, loop_group.size());
      ASSERT_EQ(1, loop_group[0].size());
   }
   {
      // DFS途中のノードが始点になる場合
      // 1-2-3, 3-2
      int N = 3;
      vector<vector<int>> adj_list(N + 1);
      vector<Edge> edge_list{
          Edge(1, 2), Edge(2, 3), Edge(3, 2)};

      for (auto [u, v] : edge_list) {
         adj_list[u].emplace_back(v);
      }

      auto loop_group = FindLoopDirected(N, edge_list);
      ASSERT_EQ(1, loop_group.size());
      ASSERT_EQ(2, loop_group[0].size());
      ASSERT_EQ(1, loop_group[0][0]);
      ASSERT_EQ(2, loop_group[0][1]);
   }
}
