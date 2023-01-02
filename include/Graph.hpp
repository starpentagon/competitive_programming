#pragma once

#include <vector>
#include <queue>
#include <iostream>

using namespace std;

void DirectedEdgeLoad() {
   int N, M;
   // [Start] Directed edge load
   // [Prefix] g-load-directed-edge-inline
   // N: nodes, M: edges
   vector<vector<int>> adj_list(N + 1, vector<int>());

   for (int i = 0; i < M; i++) {
      int u, v;
      cin >> u >> v;

      adj_list[u].emplace_back(v);
   }
   // [End] Directed edge load
}

void UndirectedEdgeLoad() {
   int N, M;
   // [Start] Undirected edge load
   // [Prefix] g-load-undirected-edge-inline
   // N: nodes, M: edges
   vector<vector<int>> adj_list(N + 1, vector<int>());

   for (int i = 0; i < M; i++) {
      int u, v;
      cin >> u >> v;

      adj_list[u].emplace_back(v);
      adj_list[v].emplace_back(u);
   }
   // [End] Undirected edge load
}

void DirectedWeightEdgeLoad() {
   int N, M;
   // [Start] Directed weight edge load
   // [Prefix] g-load-directed-weight-edge-inline
   // N: nodes, M: edges
   using Edge = pair<int, long long>;  // to, weight
   vector<vector<Edge>> adj_list(N + 1, vector<Edge>());

   for (int i = 0; i < M; i++) {
      int u, v;
      cin >> u >> v;

      long long w;
      cin >> w;

      adj_list[u].emplace_back(v, w);
   }
   // [End] Directed weight edge load
}

void UndirectedWeightEdgeLoad() {
   int N, M;
   // [Start] Undirected weight edge load
   // [Prefix] g-load-directed-weight-edge-inline
   // N: nodes, M: edges
   using Edge = tuple<int, long long>;  // to, weight
   vector<vector<Edge>> adj_list(N + 1, vector<Edge>());

   for (int i = 0; i < M; i++) {
      int u, v;
      cin >> u >> v;

      long long w;
      cin >> w;

      adj_list[u].emplace_back(v, w);
      adj_list[v].emplace_back(u, w);
   }
   // [End] Undirected weight edge load
}

// [Start] Shortest path(BFS)
// [Prefix] g-shortest-bfs
// BFSで最短路を求める
// 計算量: O(N+E)
// 非連結成分には numeric_limits<long long>::max() が設定される
// [Verified] N<=10^2, ALDS1_11_C(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_11_C&lang=ja)
vector<long long> ShortestPathBFS(const int N, const vector<vector<int>>& adj_list, const int start) {
   // 重みリストの初期化
   constexpr long long INF = numeric_limits<long long>::max();
   vector<long long> min_weight_list(N + 1, INF);

   min_weight_list[start] = 0;

   // 最短路が求まったノードを管理する
   queue<int> node_queue;
   node_queue.push(start);

   while (!node_queue.empty()) {
      const auto min_node = node_queue.front();
      node_queue.pop();

      // 隣接するノードうち未訪問のものを更新する
      for (const auto node_to : adj_list[min_node]) {
         if (min_weight_list[node_to] == INF) {
            min_weight_list[node_to] = min_weight_list[min_node] + 1;
            node_queue.push(node_to);
         }
      }
   }

   return min_weight_list;
}
// [End] Shortest path(BFS)
