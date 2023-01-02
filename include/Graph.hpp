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
// BFSで単一始点最短路を求める
// 計算量: O(N+E)
// 非連結成分には numeric_limits<long long>::max() が設定される
// [Verified] N<=10^2, ALDS1_11_C(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_11_C&lang=ja)
vector<long long> ShortestPathBFS(const vector<vector<int>>& adj_list, const int start) {
   // 重みリストの初期化
   constexpr long long INF = numeric_limits<long long>::max();
   int L = (int)adj_list.size();
   vector<long long> min_weight_list(L, INF);

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

// [Start] Shortest path(Dijkstra)
// [Prefix] g-shortest-dijkstra
// [Verified] N<=10^5, E<=5*10^5, GRL_1_A(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_A&lang=ja)
// ダイクストラ法で単一始点最短路を求める
// @pre 各エッジの重みが非負であること
// 計算量: O(E + N log N)
// 非連結成分には numeric_limits<long long>::max() が設定される
vector<long long> ShortestPathDijkstra(const vector<vector<pair<int, long long>>>& adj_list, const int start) {
   // 重みリストの初期化
   int L = (int)adj_list.size();
   constexpr long long INF = numeric_limits<long long>::max();
   vector<long long> min_weight_list(L, INF);

   // 重み最小のノードを管理
   using WeightNode = pair<long long, int>;  // (startからの最小重み, ノード番号)
   priority_queue<WeightNode, vector<WeightNode>, greater<WeightNode>> node_queue;

   min_weight_list[start] = 0;
   node_queue.emplace(0, start);

   while (!node_queue.empty()) {
      const auto [min_weight, min_node] = node_queue.top();
      node_queue.pop();

      // すでに更新済みの場合はskip
      // - skipしないとO(N^2)となるケースが存在
      // see: https://snuke.hatenablog.com/entry/2021/02/22/102734
      if (min_weight_list[min_node] < min_weight) {
         continue;
      }

      // 重み最小のノードに隣接するノードを更新できるかチェック
      for (const auto& [node_to, weight] : adj_list[min_node]) {
         if (min_weight_list[node_to] > min_weight + weight) {
            min_weight_list[node_to] = min_weight + weight;
            node_queue.emplace(min_weight_list[node_to], node_to);
         }
      }
   }

   return min_weight_list;
}
// [End] Shortest path(Dijkstra)

// [Start] Shortest path(BellmanFord)
// [Prefix] g-shortest-bellman
// [Verified] N<=10^3, E<=2*10^3, GRL_1_B(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_B&lang=ja)
// ベルマン・フォード法で単一始点最短路を求める
// @note 負の重みが存在しても正しく動く
// @note 負の閉路があれば検知可能
// @retval true 負の閉路が存在, false 負の閉路が存在しない
// @note 負の閉路が存在する場合、重みがマイナス無限大になるノードをDetectNegtiveInfNode()で特定可能
// @note 計算量: O(E * N)
// 非連結成分には numeric_limits<long long>::max() が設定される
pair<bool, vector<long long>> ShortestPathBellmanFord(const vector<vector<pair<int, long long>>>& adj_list, const int start) {
   // 重みリストの初期化
   int N = (int)adj_list.size() - 1;
   constexpr long long INF = numeric_limits<long long>::max();
   vector<long long> min_weight_list(N + 1, INF);

   min_weight_list[start] = 0;

   // 辺のリスト
   using Edge = tuple<int, int, long long>;
   vector<Edge> edge_list;

   for (int i = 1; i <= N; i++) {
      for (auto [to, weight] : adj_list[i]) {
         edge_list.emplace_back(i, to, weight);
      }
   }

   bool negative_loop = false;

   for (int i = 1; i <= N; i++) {
      for (const auto [from, to, weight] : edge_list) {
         if (min_weight_list[from] == INF) {
            // 重み < 0の可能性があり直後の判定式inf > inf - dで成立し意図しない挙動になるため明示的にスキップ
            continue;
         }

         if (min_weight_list[to] > min_weight_list[from] + weight) {
            min_weight_list[to] = min_weight_list[from] + weight;

            if (i == N) {
               // 負の閉路がない場合はN-1回目のループで最短経路が求まる
               // つまり、N回目のループで値の更新が起こるのは負の閉路がある場合
               negative_loop = true;
               break;
            }
         }
      }
   }

   return {negative_loop, min_weight_list};
}

// 最短路の重みが負の無限大になるノードを特定する
// @param min_weight_list : ShortestPathBellmanFord()を実行済の重みリスト
// @param negative_inf_node[i] = true : ノードiの重みが負の無限大
vector<bool> DetectNegativeInfNode(const vector<vector<pair<int, long long>>>& adj_list, const int start, vector<long long>& min_weight_list) {
   // 重みリストの初期化
   int N = (int)adj_list.size() - 1;
   constexpr long long INF = numeric_limits<long long>::max();

   // 負の無限大フラグリストの初期化
   vector<bool> negative_inf_node(N + 1, false);

   // 辺のリスト
   using Edge = tuple<int, int, long long>;
   vector<Edge> edge_list;

   for (int i = 1; i <= N; i++) {
      for (auto [to, weight] : adj_list[i]) {
         edge_list.emplace_back(i, to, weight);
      }
   }

   for (size_t i = 1; i <= N; i++) {
      for (const auto [from, to, weight] : edge_list) {
         if (min_weight_list[from] == INF) {
            continue;
         }

         if (min_weight_list[to] > min_weight_list[from] + weight) {
            // N+1回目以降で値の更新が起きているので繰り返すことで負の無限大へ
            min_weight_list[to] = min_weight_list[from] + weight;
            negative_inf_node[to] = true;
         }

         // 負の無限大になるノードから遷移可能
         if (negative_inf_node[from]) {
            negative_inf_node[to] = true;
         }
      }
   }

   return negative_inf_node;
}
// [End] Shortest path(BellmanFord)

// [Start] Shortest path(WarshallFloyd)
// [Prefix] g-shortest-warshall
// [Verified] N<=10^2, GRL_1_C(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_C&lang=ja)
// ワーシャル・フロイド法で全点対間最短路を求める
// @note 負の重みが存在しても正しく動く
// @param min_weight_list[i][j] : node i->jの最短路。1-indexedのためi=0, j=0にはINFが入る
// @retval true 負の閉路が存在, false 負の閉路が存在しない
// @note 最短路の重みが負の無限大になるノードはmin_weight_list[node][node]が負のもの
// @note 計算量: O(N^3)
// 非連結成分には INF = numeric_limits<long long>::max() が設定される
pair<bool, vector<vector<long long>>> AllShortestPathWarshallFloyd(const vector<vector<pair<int, long long>>>& adj_list) {
   // dp[k][i][j]: ノード1からkまでを使ってiからjまで移った時の最小重み
   // -> dp[i][j]のみで管理可能
   int L = (int)adj_list.size();
   int N = L - 1;
   constexpr long long INF = numeric_limits<long long>::max();

   vector<vector<long long>> min_weight_list(L, vector<long long>(L, INF));

   // 初期化
   for (int node = 1; node <= N; node++) {
      min_weight_list[node][node] = 0;

      for (const auto [to, weight] : adj_list[node]) {
         min_weight_list[node][to] = weight;
      }
   }

   for (int node_k = 1; node_k <= N; node_k++) {
      for (int node_i = 1; node_i <= N; node_i++) {
         for (int node_j = 1; node_j <= N; node_j++) {
            if (min_weight_list[node_i][node_k] == INF || min_weight_list[node_k][node_j] == INF) {
               // 重み < 0の可能性があり直後の更新がinf > inf - dで成立し意図しない挙動になるため明示的にスキップ
               continue;
            }

            long long weight = min_weight_list[node_i][node_k] + min_weight_list[node_k][node_j];

            min_weight_list[node_i][node_j] = min(min_weight_list[node_i][node_j], weight);
         }
      }
   }

   bool negative_loop = false;

   for (int node = 1; node <= N; node++) {
      if (min_weight_list[node][node] < 0) {
         negative_loop = true;
      }
   }

   return {negative_loop, min_weight_list};
}

// [End] Shortest path(WarshallFloyd)
