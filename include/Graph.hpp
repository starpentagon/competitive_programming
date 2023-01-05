#pragma once

#include <algorithm>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <iostream>

#include "UnionFind.hpp"

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

// [Start] Find shortest path
// [Prefix] g-find-shortest-path-func
// ノード間の最短経路を求める(重み付き用)
// @param start, node: 最短経路を求めるノード
// @param min_weight_list: startから各ノードの最短距離が格納されたテーブル
// @retval node_path: startノードからendノードまでの最短経路順に並べたノードリスト
// @note 計算量: O(E)
// Unverified
vector<int> FindShortestPath(const int start, const int end, const vector<vector<pair<int, long long>>>& adj_list, const vector<long long>& min_weight_list) {
   int N = (int)adj_list.size() - 1;

   // to -> fromの逆順隣接リストを作る
   using Edge = pair<int, long long>;
   vector<vector<Edge>> rev_adj_list(N + 1, vector<Edge>());

   for (int from = 1; from <= N; from++) {
      for (auto [to, weight] : adj_list[from]) {
         rev_adj_list[to].emplace_back(from, weight);
      }
   }

   vector<int> back_path;
   int node = end;

   back_path.push_back(node);

   while (min_weight_list[node] != 0) {
      for (auto [from, weight] : rev_adj_list[node]) {
         if (min_weight_list[node] == min_weight_list[from] + weight) {
            back_path.push_back(from);
            node = from;
            break;
         }
      }
   }

   reverse(back_path.begin(), back_path.end());
   return back_path;
}

// ノード間の最短経路を求める(重みなし用)
// @param start, node: 最短経路を求めるノード
// @param min_weight_list: startから各ノードの最短距離が格納されたテーブル
// @retval node_path: startノードからendノードまでの最短経路順に並べたノードリスト
// @note 計算量: O(E)
vector<int> FindShortestPath(const int start, const int end, const vector<vector<int>>& adj_list, const vector<long long>& min_weight_list) {
   int N = (int)adj_list.size() - 1;

   using Edge = pair<int, long long>;
   vector<vector<Edge>> weight_adj_list(N + 1, vector<Edge>());

   for (int node = 1; node <= N; node++) {
      for (auto to : adj_list[node]) {
         weight_adj_list[node].emplace_back(to, 1);
      }
   }

   return FindShortestPath(start, end, weight_adj_list, min_weight_list);
}
// [End] Find shortest path

// [Start] Topological Sort
// [Prefix] g-topological-sort-func
// [Verified] N<=10^4, E<=10^5, GRL_4_B(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_4_B&lang=ja)
// 隣接リストからトポロジカルソートを行う
// @param adj_list 隣接リスト
// @retval sorted_list : トポロジカルソート後のノード番号リスト
// @retval true トポロジカルソート完了, false DAGでなくトポロジカルソート不可能
// @note 計算量: O(E + N)
pair<bool, vector<int>> TopologicalSort(const vector<vector<int>>& adj_list) {
   const int N = (int)adj_list.size() - 1;

   using Edge = pair<int, int>;  // from, to
   vector<Edge> edge_list;

   for (int node = 1; node <= N; node++) {
      for (auto to : adj_list[node]) {
         edge_list.emplace_back(node, to);
      }
   }

   vector<int> sorted_list;
   sorted_list.reserve(N);

   // ノードの入力次数
   vector<int> in_degree(N + 1, 0);

   // 隣接リスト(エッジインデックスのリスト)を作成
   vector<vector<int>> edge_index_list(N + 1, vector<int>());
   const int E = (int)edge_list.size();

   for (int i = 0; i < E; i++) {
      const auto [from, to] = edge_list[i];

      in_degree[to]++;
      edge_index_list[from].push_back(i);
   }

   // エッジが残っているかのフラグ
   vector<bool> edge_exist(E, true);

   // 入力次数が0のノードが始点になる
   queue<int> node_queue;

   for (int node = 1; node <= N; node++) {
      if (in_degree[node] == 0) {
         node_queue.push(node);
      }
   }

   while (!node_queue.empty()) {
      int node = node_queue.front();
      node_queue.pop();

      sorted_list.push_back(node);

      for (int edge_index : edge_index_list[node]) {
         if (!edge_exist[edge_index]) {
            continue;
         }

         edge_exist[edge_index] = false;

         const auto [from, to] = edge_list[edge_index];
         in_degree[to]--;

         if (in_degree[to] == 0) {
            node_queue.push(to);
         }
      }
   }

   bool all_edge_deleted = true;

   for (int i = 0; i < E; i++) {
      if (edge_exist[i]) {
         all_edge_deleted = false;
         break;
      }
   }

   return {all_edge_deleted, sorted_list};
}

// 重み付き版
pair<bool, vector<int>> TopologicalSort(const vector<vector<pair<int, long long>>>& adj_weight_list) {
   int N = adj_weight_list.size() - 1;
   vector<vector<int>> adj_list(N + 1);

   for (int u = 1; u <= N; u++) {
      for (auto [v, w] : adj_weight_list[u]) {
         adj_list[u].emplace_back(v);
      }
   }

   return TopologicalSort(adj_list);
}
// [End] Topological Sort

// [Start] Loop detection(Undirected)
// [Prefix] g-loop-undirected-detection
// [Verified] N<=10, Unit test(単一連結成分でloopあり/なし, 複数連結成分でloopありなし混在)
// [Verified] N, E<=5*10^5, 「Cycle Detection (Undirected)」(https://judge.yosupo.jp/problem/cycle_detection_undirected)
// 無向グラフで閉路を求める
// @retval 閉路のリスト。閉路は(start_node, edge index list)のpairで表現
// @note 連結成分ごとに閉路があればそのうち1つを返す
// @note 自己ループ、多重辺があっても可能
// 計算量: ほぼO(N + E)
// 依存ライブラリ: UnionFind, Shortest path(BFS), Find shortest path
vector<pair<int, vector<int>>> FindLoopUndirected(int N, const vector<pair<int, int>>& edge_list) {
   UnionFind uf(N);

   vector<bool> root_loop(N + 1, false);
   vector<vector<int>> adj_list(N + 1);
   int M = edge_list.size();
   vector<unordered_map<int, int>> node_edge_map(N + 1);  // node_edge_map[i][j] -> edge(i, j)のindex
   vector<pair<int, vector<int>>> loop_group;

   auto Unite = [&](int edge_index) {
      auto [u, v] = edge_list[edge_index];

      uf.Unite(u, v);

      adj_list[u].emplace_back(v);
      adj_list[v].emplace_back(u);

      node_edge_map[u][v] = edge_index;
      node_edge_map[v][u] = edge_index;
   };

   for (int i = 0; i < M; i++) {
      auto [u, v] = edge_list[i];

      auto root_u = uf.root(u);
      auto root_v = uf.root(v);

      if (root_loop[root_u] || root_loop[root_v]) {
         // すでにloopを抽出済
         Unite(i);
         root_loop[root_u] = root_loop[root_v] = true;
         continue;
      }

      if (uf.IsSameGroup(u, v)) {
         // u -> vのpathを求める
         auto min_dist_tbl = ShortestPathBFS(adj_list, u);
         auto loop_node_list = FindShortestPath(u, v, adj_list, min_dist_tbl);
         vector<int> loop_edge_index_list;

         for (int l = 1; l < (int)loop_node_list.size(); l++) {
            auto from = loop_node_list[l - 1];
            auto to = loop_node_list[l];

            auto e = node_edge_map[from][to];
            loop_edge_index_list.emplace_back(e);
         }

         // u-vをつないで閉路を作る
         loop_edge_index_list.emplace_back(i);
         root_loop[root_u] = root_loop[root_v] = true;

         loop_group.emplace_back(u, loop_edge_index_list);
      }

      Unite(i);
   }

   return loop_group;
}
// [End] Loop detection(Undirected)

// [Start] Loop detection(Directed)
// [Prefix] g-loop-directed-detection
// [Verified] N, E<=5*10^5, 「Cycle Detection (Directed)」(https://judge.yosupo.jp/problem/cycle_detection)
// ToDo: 「E - Takahashi's Anguish」(https://atcoder.jp/contests/abc256/tasks/abc256_e)でのverify
// 有向グラフで閉路を求める
// @retval 閉路のリスト。閉路はmap[node]: to_nodeのマップで表現
// @note 連結成分ごとに閉路があればそのうち1つを返す
// @note 自己ループがあっても可能
// @retval 閉路のリスト。閉路はedge index listのlistで表現
vector<vector<int>> FindLoopDirected(int N, const vector<pair<int, int>>& edge_list) {
   using EdgeInfo = pair<int, int>;  // to, edge index
   vector<vector<EdgeInfo>> adj_list(N + 1);
   int M = edge_list.size();

   for (int i = 0; i < M; i++) {
      auto [u, v] = edge_list[i];

      adj_list[u].emplace_back(v, i);
   }

   vector<bool> visited(N + 1, false);   // 1-indexed
   vector<bool> finished(N + 1, false);  // 1-indexed

   // @retval -1: loopはない, 0: loop復元済: 1以上: ループ復元中でループの開始ノード番号
   auto dfs = [&](auto dfs, int node, vector<int>& loop_edge_index_list) -> int {
      int ret = -1;
      visited[node] = true;

      for (auto [n_node, edge_index] : adj_list[node]) {
         if (finished[n_node]) continue;

         if (visited[n_node] && !finished[n_node]) {
            // n_nodeを開始ノードとするloopが存在する
            loop_edge_index_list.emplace_back(edge_index);
            ret = n_node;

            if (n_node == node) ret = 0;
            break;
         }

         ret = dfs(dfs, n_node, loop_edge_index_list);

         if (ret != -1) {
            if (ret >= 1) {
               loop_edge_index_list.emplace_back(edge_index);
            }

            if (ret == node) {
               ret = 0;
            }
            break;
         }
      }

      finished[node] = true;
      return ret;
   };

   vector<vector<int>> loop_group;

   for (int node = 1; node <= N; node++) {
      vector<int> loop_edge_index_list;
      auto loop_node = dfs(dfs, node, loop_edge_index_list);

      if (loop_node != -1) {
         reverse(loop_edge_index_list.begin(), loop_edge_index_list.end());
         loop_group.emplace_back(loop_edge_index_list);
      }
   }

   return loop_group;
}
// [End] Loop detection(Directed)

// [Start] Longest path
// [Prefix] g-longest-path-func
// [Verified] N, E<=10^5, EDPC「G - Longest Path」(https://atcoder.jp/contests/dp/tasks/dp_g)
// DAGにおける最長路を求める
// @param adj_list 重み付き隣接リスト
// @retval -1 : DAGでなく最長路を求められない, 0以上 : 最長路の長さ
// @note 計算量 : O(E + N)
// 依存ライブラリ: Topological Sort
long long LongestPath(const vector<vector<pair<int, long long>>>& adj_weight_list) {
   const int N = adj_weight_list.size() - 1;
   vector<long long> longest_path_weight(N + 1, 0);

   // トポロジカルソート順に見ると自ノードへの到達は今後ないので最長距離を確定できる
   auto [is_dag, sorted_nodes] = TopologicalSort(adj_weight_list);

   if (!is_dag) {
      return -1;
   }

   for (auto from : sorted_nodes) {
      for (auto& [to, weight] : adj_weight_list[from]) {
         if (longest_path_weight[to] < longest_path_weight[from] + weight) {
            longest_path_weight[to] = longest_path_weight[from] + weight;
         }
      }
   }

   auto longest_path = *max_element(longest_path_weight.begin(), longest_path_weight.end());
   return longest_path;
}

// 重みなし版
long long LongestPath(const vector<vector<int>>& adj_list) {
   int N = adj_list.size() - 1;
   vector<vector<pair<int, long long>>> adj_weight_list(N + 1);

   for (int u = 1; u <= N; u++) {
      for (auto v : adj_list[u]) {
         adj_weight_list[u].emplace_back(v, 1);
      }
   }

   return LongestPath(adj_weight_list);
}
// [End] Longest path

// [Start] Move on loop
// [Prefix] g-move-on-loop-func
// [Verified] N,Q <= 10^5: ABC_258「E - Packing Potatoes」(https://atcoder.jp/contests/abc258/tasks/abc258_e)
// 指定回数移動後にどのノードにいるかを返す
// @note 事前計算: O(N), startからMoveCount回の移動計算: O(1)
// @pre ノードstartが所属する連結成分はFunctional Graphであること
// @retval [out_loop, in_loop] : startを起点にloopに入るまでの移動履歴、loopに入った後の移動履歴
pair<vector<int>, vector<int>> MoveOnLoopPrep(const int start, const vector<vector<int>>& adj_list, const vector<pair<int, int>>& edge_list, const vector<int>& loop_edge_index_list) {
   int N = adj_list.size() - 1;
   vector<bool> loop_node(N + 1, false);

   for (auto e : loop_edge_index_list) {
      auto [from, to] = edge_list[e];
      loop_node[from] = true;
   }

   int node = start;
   auto loop_size = loop_edge_index_list.size();

   vector<int> out_loop_node, in_loop_node;

   while (true) {
      if (loop_node[node]) {
         if (in_loop_node.size() == loop_size) {
            break;
         }

         in_loop_node.emplace_back(node);
      } else {
         out_loop_node.emplace_back(node);
      }

      assert(adj_list[node].size() == 1);
      node = adj_list[node][0];
   }

   return {out_loop_node, in_loop_node};
}

// 指定回数を移動した後のノード番号を返す
int MoveOnLoop(const vector<int>& out_loop_node, const vector<int>& in_loop_node, long long move_count) {
   if (move_count < (long long)out_loop_node.size()) {
      return out_loop_node[move_count];
   }

   move_count -= out_loop_node.size();
   move_count %= in_loop_node.size();

   return in_loop_node[move_count];
}
// [End] Move on loop
