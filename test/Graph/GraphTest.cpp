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

TEST(GraphTest, TestEulerTourSubTree) {
   vector<EulerTour::Edge> adj_list;

   // 参考: https://maspypy.com/euler-tour-%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7
   adj_list.emplace_back(1, 2, 1);
   adj_list.emplace_back(2, 3, 2);
   adj_list.emplace_back(3, 4, 3);
   adj_list.emplace_back(2, 5, 4);
   adj_list.emplace_back(1, 6, 5);

   EulerTour euler_tour(adj_list);
   euler_tour.SetNodeWeight(5, 100);
   euler_tour.Build(1);

   EXPECT_EQ(6, euler_tour.GetSubTreeSize(1));
   EXPECT_EQ(4, euler_tour.GetSubTreeSize(2));
   EXPECT_EQ(2, euler_tour.GetSubTreeSize(3));
   EXPECT_EQ(1, euler_tour.GetSubTreeSize(4));
   EXPECT_EQ(1, euler_tour.GetSubTreeSize(5));
   EXPECT_EQ(1, euler_tour.GetSubTreeSize(6));

   EXPECT_EQ(2 + 3 + 4 + 100, euler_tour.SubTreeQuery(2));
   EXPECT_EQ(3, euler_tour.SubTreeQuery(3));
}

TEST(GraphTest, TestEulerPath) {
   vector<EulerTour::Edge> adj_list;

   // 参考: https://maspypy.com/euler-tour-%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7
   adj_list.emplace_back(1, 2, 1);
   adj_list.emplace_back(2, 3, 2);
   adj_list.emplace_back(3, 4, 3);
   adj_list.emplace_back(2, 5, 4);
   adj_list.emplace_back(1, 6, 5);

   EulerTour euler_tour(adj_list);
   euler_tour.SetNodeWeight(5, 100);
   euler_tour.Build(1);

   EXPECT_EQ(0, euler_tour.PathQuery(1));
   EXPECT_EQ(1, euler_tour.PathQuery(2));
   EXPECT_EQ(3, euler_tour.PathQuery(3));
   EXPECT_EQ(6, euler_tour.PathQuery(4));
   EXPECT_EQ(105, euler_tour.PathQuery(5));
   EXPECT_EQ(5, euler_tour.PathQuery(6));
}

TEST(GraphTest, TestEulerLCA) {
   vector<EulerTour::Edge> adj_list;

   // 参考: https://maspypy.com/euler-tour-%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7
   adj_list.emplace_back(1, 2, 1);
   adj_list.emplace_back(2, 3, 2);
   adj_list.emplace_back(3, 4, 3);
   adj_list.emplace_back(2, 5, 4);
   adj_list.emplace_back(1, 6, 5);

   EulerTour euler_tour(adj_list);
   euler_tour.SetNodeWeight(5, 100);
   euler_tour.SetNodeWeight(1, 10);

   euler_tour.Build(1);

   // 深さ
   EXPECT_EQ(0, euler_tour.GetDepth(1));
   EXPECT_EQ(1, euler_tour.GetDepth(2));
   EXPECT_EQ(2, euler_tour.GetDepth(3));
   EXPECT_EQ(3, euler_tour.GetDepth(4));
   EXPECT_EQ(2, euler_tour.GetDepth(5));
   EXPECT_EQ(1, euler_tour.GetDepth(6));

   // LCA
   EXPECT_EQ(1, euler_tour.GetLCA(1, 1));
   EXPECT_EQ(2, euler_tour.GetLCA(3, 5));
   EXPECT_EQ(2, euler_tour.GetLCA(5, 4));
   EXPECT_EQ(1, euler_tour.GetLCA(3, 6));
   EXPECT_EQ(3, euler_tour.GetLCA(3, 4));

   // 2点間のクエリ
   EXPECT_EQ(2 + 1 + 10 + 5, euler_tour.PathQuery(3, 6));
   EXPECT_EQ(100 + 4 + 1 + 10 + 5, euler_tour.PathQuery(5, 6));
   EXPECT_EQ(100 + 4 + 2 + 3, euler_tour.PathQuery(5, 4));

   // 辺の重みを更新
   euler_tour.UpdateEdgeWeight(1, 20);  // 2 -> 3: 20

   // 2点間のクエリ
   EXPECT_EQ(20 + 1 + 10 + 5, euler_tour.PathQuery(3, 6));
   EXPECT_EQ(100 + 4 + 1 + 10 + 5, euler_tour.PathQuery(5, 6));
   EXPECT_EQ(100 + 4 + 20 + 3, euler_tour.PathQuery(5, 4));

   // 頂点の重みを更新
   euler_tour.UpdateNodeWeight(1, 20);  // 1: 20

   EXPECT_EQ(20 + 1 + 20 + 5, euler_tour.PathQuery(3, 6));
   EXPECT_EQ(100 + 4 + 1 + 20 + 5, euler_tour.PathQuery(5, 6));
   EXPECT_EQ(100 + 4 + 20 + 3, euler_tour.PathQuery(5, 4));
}

TEST(GraphTest, TestEulerInSubtree) {
   vector<EulerTour::Edge> adj_list;

   // 参考: https://maspypy.com/euler-tour-%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7
   adj_list.emplace_back(1, 2, 1);
   adj_list.emplace_back(2, 3, 2);
   adj_list.emplace_back(3, 4, 3);
   adj_list.emplace_back(2, 5, 4);
   adj_list.emplace_back(1, 6, 5);

   EulerTour euler_tour(adj_list);
   euler_tour.Build(1);

   EXPECT_TRUE(euler_tour.InSubtree(1, 5));
   EXPECT_FALSE(euler_tour.InSubtree(2, 1));
   EXPECT_TRUE(euler_tour.InSubtree(3, 4));
   EXPECT_TRUE(euler_tour.InSubtree(4, 4));
   EXPECT_FALSE(euler_tour.InSubtree(6, 5));
}

TEST(GraphTest, TestEulerPathLength) {
   vector<EulerTour::Edge> adj_list;

   // 参考: https://maspypy.com/euler-tour-%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7
   adj_list.emplace_back(1, 2, 1);
   adj_list.emplace_back(2, 3, 2);
   adj_list.emplace_back(3, 4, 3);
   adj_list.emplace_back(2, 5, 4);
   adj_list.emplace_back(1, 6, 5);

   EulerTour euler_tour(adj_list);
   euler_tour.Build(1);

   EXPECT_EQ(0, euler_tour.GetPathLength(2, 2));
   EXPECT_EQ(1, euler_tour.GetPathLength(1, 2));
   EXPECT_EQ(4, euler_tour.GetPathLength(4, 6));
   EXPECT_EQ(3, euler_tour.GetPathLength(6, 5));
}