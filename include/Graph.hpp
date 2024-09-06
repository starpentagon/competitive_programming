#pragma once

#include <algorithm>
#include <cassert>
#include <vector>
#include <set>
#include <queue>
#include <stack>
#include <unordered_map>
#include <iostream>
#include <limits>

#include "UnionFind.hpp"
#include "SegmentTree.hpp"
#include "SparseTable.hpp"

using namespace std;

void DirectedEdgeLoad() {
   int N = 0, M = 0;
   // clang-format off
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
   // clang-format on
}

void UndirectedEdgeLoad() {
   int N = 0, M = 0;
   // clang-format off
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
   // clang-format on
}

void DirectedWeightEdgeLoad() {
   int N = 0, M = 0;
   // clang-format off
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
   // clang-format on
}

void UndirectedWeightEdgeLoad() {
   int N = 0, M = 0;
   // clang-format off
   // [Start] Undirected weight edge load
   // [Prefix] g-load-undirected-weight-edge-inline
// N: nodes, M: edges
using Edge = pair<int, long long>;  // to, weight
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
   // clang-format on
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
// [Verified] N, E<=5*10^5: Shortest Path(https://judge.yosupo.jp/problem/shortest_path)
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
      for (const auto& [from, to, weight] : edge_list) {
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

   for (int i = 1; i <= N; i++) {
      for (const auto& [from, to, weight] : edge_list) {
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

      for (const auto& [to, weight] : adj_list[node]) {
         // 多重辺があるため最小の重みを採用
         min_weight_list[node][to] = min(min_weight_list[node][to], weight);
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
// [Verified] N, E<=5*10^5: Shortest Path(https://judge.yosupo.jp/problem/shortest_path)
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
   vector<bool> visited(N + 1, false);
   constexpr long long INF = numeric_limits<long long>::max();

   int node = end;

   back_path.push_back(node);
   visited[node] = true;

   while (node != start) {
      for (auto [from, weight] : rev_adj_list[node]) {
         if (visited[from] || min_weight_list[from] == INF) continue;

         if (min_weight_list[node] == min_weight_list[from] + weight) {
            back_path.push_back(from);
            node = from;
            visited[from] = true;
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

// [Start] Euler Tour
// [Prefix] g-euler-tour-class
// EulerTourによる木クエリを行う
// 依存ライブラリ: セグメント木(seg-tree-class), SparseTable(sp-table-class)
// @note 参考: maspyのHP「Euler Tour のお勉強」(https://maspypy.com/euler-tour-%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7)
class EulerTour {
  public:
   using Edge = tuple<int, int, long long>;  // node_1, node_2, weight
   EulerTour(const vector<Edge>& edge_list);

   // ノードの重みを設定する
   // @param node ノード番号(1-index)
   // @param weight 重み
   void SetNodeWeight(const int node, const long long weight);

   // ノードの重みを取得する
   long long GetNodeWeight(const int node) const;

   // 辺の重みを取得する
   long long GetEdgeWeight(const int edge_index) const;

   // ノードの重みを更新する
   // 計算量: O(log N)
   // [Verified] N, Q <= 5 * 10^5: Vertex Add Subtree Sum(https://judge.yosupo.jp/problem/vertex_add_subtree_sum)
   // [Verified] N, Q <= 5 * 10^5 Vertex Add Path Sum(https://judge.yosupo.jp/problem/vertex_add_path_sum)
   void UpdateNodeWeight(const int node, const long long weight);

   // 辺の重みを更新する
   // 計算量: O(log N)
   // [Verified] N <= 10^5, Q <= 2 * 10^5, Range Query on a Tree(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_D&lang=ja)
   // [Verified] N <= 2 * 10^5, Q <= 2 * 10^5, ABC294「G - Distance Queries on a Tree」(https://atcoder.jp/contests/abc294/tasks/abc294_g)
   void UpdateEdgeWeight(int edge_index, const long long weight);

   // Euler tour順序で部分木クエリ/パスクエリ/LCA/2点間クエリ用のデータを生成する
   // 計算量: O(N log N)
   // @note LCAが不要ならばSparseTableの生成をskipすることでO(N)に高速化可能
   void Build(int root_node);

   // 部分木のノード数を返す
   // @note 木なので辺数はノード数 - 1
   // 計算量: O(1)
   int GetSubTreeSize(const int node) const;

   // sub_tree_rootを根とする部分木にnodeが含まれているかを返す
   // 計算量: O(1)
   bool InSubtree(const int sub_tree_root, const int node) const;

   // 部分木クエリ(ノードと辺の重みの合計)を返す
   // @param node ノード番号(1-index)
   // 計算量: O(log N)
   // [Verified] N, Q <= 5 * 10^5: Vertex Add Subtree Sum(https://judge.yosupo.jp/problem/vertex_add_subtree_sum)
   long long SubTreeQuery(const int node) const;

   // パスクエリ(rootからノードへの合計)を返す
   // 計算量: O(log N)
   // [Verified] N <= 10^5, Q <= 2 * 10^5, Range Query on a Tree(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_D&lang=ja)
   long long PathQuery(const int node) const;

   // ノードの深さを返す
   // 計算量: O(1)
   int GetDepth(const int node) const;

   // 2ノード間の最近共通祖先(LCA)を求める
   // 計算量: O(1)
   // [Verified] N, Q <= 5 * 10^5: Lowest Common Ancestor(https://judge.yosupo.jp/problem/lca)
   int GetLCA(const int node_1, const int node_2) const;

   // 2ノード間のパスの長さ(辺の数)を求める
   // 計算量: O(1)
   int GetPathLength(const int node_1, const int node_2) const;

   // 2ノード間のパスクエリを返す(node_1からnode_2への最短路上の合計を返す)
   // 計算量: O(log N)
   // [Verified] N, Q <= 5 * 10^5 Vertex Add Path Sum(https://judge.yosupo.jp/problem/vertex_add_path_sum)
   // [Verified] N <= 2 * 10^5, Q <= 2 * 10^5, ABC294「G - Distance Queries on a Tree」(https://atcoder.jp/contests/abc294/tasks/abc294_g)
   long long PathQuery(const int node_1, const int node_2) const;

  protected:
   // nodeをrootとする部分木の範囲([in_time, out_time])を返す
   pair<int, int> GetInOutTime(int node) const;

   int N_;  // 頂点数

   bool build_;     // クエリ前にBuildが行われているかのフラグ
   int root_node_;  // root node

   vector<long long> node_weight_;  // node -> node weight

   vector<vector<pair<int, long long>>> adj_list_;  // 隣接リスト(from -> to, weight)
   vector<Edge> edge_list_;                         // 辺リスト(from, to, weight)

   vector<int> node_in_time_;   // nodeごとの行きがけ時の訪問時刻(最初にそのノードを通った時刻)
   vector<int> node_out_time_;  // nodeごとの帰りがけ時の時刻(最後にそのノードを通った時刻)

   function<long long(long long, long long)> sum_oper_;  // セグメント木用の加算オペレータ

   long long GetDepthNodeIndex(int depth, int node) const;  // (depth, node) -> index
   pair<int, int> GetDepthNode(long long index) const;      // index -> (depth, node)

   function<long long(long long, long long)> min_oper_;  // LCA用のminオペレータ

   SegmentTree<long long> seg_tree_subtree_node_weight_;  // 部分木クエリ用のセグメント木(ノードの重み)
   SegmentTree<long long> seg_tree_subtree_edge_weight_;  // 部分木クエリ用のセグメント木(辺の重み)

   SegmentTree<long long> seg_tree_path_node_weight_;  // パスクエリ用のセグメント木(ノードの重み)
   SegmentTree<long long> seg_tree_path_edge_weight_;  // パスクエリ用のセグメント木(辺の重み)

   SparseTable<long long> sp_table_lca_depth_node_;  // LCA用のSparseTable(DepthNodeIndex)
};

EulerTour::EulerTour(const vector<Edge>& edge_list)
    : N_(edge_list.size() + 1), build_(false), root_node_(-1), node_weight_(N_ + 1, 0), adj_list_(N_ + 1), edge_list_(edge_list), sum_oper_([](long long x, long long y) -> long long { return x + y; }), min_oper_([](long long x, long long y) -> long long { return min(x, y); }), seg_tree_subtree_node_weight_(vector<long long>(), sum_oper_, 0), seg_tree_subtree_edge_weight_(vector<long long>(), sum_oper_, 0), seg_tree_path_node_weight_(vector<long long>(), sum_oper_, 0), seg_tree_path_edge_weight_(vector<long long>(), sum_oper_, 0), sp_table_lca_depth_node_(vector<long long>(1, 0), min_oper_) {
   for (int i = 0; i < N_ - 1; i++) {
      auto [u, v, weight] = edge_list[i];
      adj_list_[u].emplace_back(v, weight);
      adj_list_[v].emplace_back(u, weight);
   }
}

void EulerTour::SetNodeWeight(const int node, const long long weight) {
   assert(1 <= node && node <= N_);
   node_weight_[node] = weight;

   build_ = false;
}

void EulerTour::Build(int root_node) {
   assert(1 <= root_node && root_node <= N_);
   if (build_) return;
   build_ = true;
   root_node_ = root_node;

   node_in_time_.resize(N_ + 1, -1);
   node_out_time_.resize(N_ + 1, -1);

   const int kEdgeCount = N_ - 1;

   // 辺を通るごとに時刻+1, 各辺を2回通るので時刻は[0, 2*辺数], パスクエリ用に[0, 2*辺数+1]まで用意
   const int kMaxTime = 2 * kEdgeCount + 1 + 1;

   vector<long long> subtree_time_node_weight(kMaxTime, 0);  // 部分木クエリ用の時刻ごとのノードの重み(重複加算しないように行きがけ時のみ記録)
   vector<long long> subtree_time_edge_weight(kMaxTime, 0);  // 部分木クエリ用の時刻ごとの辺の重み(重複加算しないように行きがけ時のみ記録)

   vector<long long> path_time_node_weight(kMaxTime, 0);  // パスクエリ用の時刻ごとのノードの重み(行きがけ時に+, 帰りがけ時に-を記録)
   vector<long long> path_time_edge_weight(kMaxTime, 0);  // パスクエリ用の時刻ごとの辺の重み(行きがけ時に+, 帰りがけ時に-を記録)

   vector<long long> lca_depth_node_index(kMaxTime, numeric_limits<long long>::max());  // LCA用の時刻ごとの深さ/ノードindexを記録

   int t = -1;  // 時刻

   auto dfs = [&](auto dfs, int depth, int node, int p_node, long long weight) -> void {
      t++;

      // ノードに入る
      node_in_time_[node] = t;

      subtree_time_node_weight[t] = node_weight_[node];
      subtree_time_edge_weight[t] = weight;

      path_time_node_weight[t] = node_weight_[node];
      path_time_edge_weight[t] = weight;

      lca_depth_node_index[t] = GetDepthNodeIndex(depth, node);

      for (const auto& [n_node, weight] : adj_list_[node]) {
         if (n_node == p_node) continue;
         dfs(dfs, depth + 1, n_node, node, weight);
      }

      node_out_time_[node] = t;

      // ノードから抜ける
      t++;

      // パスクエリは累積和を取った時に単位元になるよう逆元を設定
      path_time_node_weight[t] = -node_weight_[node];
      path_time_edge_weight[t] = -weight;

      lca_depth_node_index[t] = GetDepthNodeIndex(depth - 1, p_node);
   };

   dfs(dfs, 0, root_node, -1, 0);

   // セグメント木の構築
   // 部分木クエリ用
   long long unit_sum = 0;
   SegmentTree<long long> seg_tree_subtree_node_weight(subtree_time_node_weight, sum_oper_, unit_sum);
   SegmentTree<long long> seg_tree_subtree_edge_weight(subtree_time_edge_weight, sum_oper_, unit_sum);

   seg_tree_subtree_node_weight_ = seg_tree_subtree_node_weight;
   seg_tree_subtree_edge_weight_ = seg_tree_subtree_edge_weight;

   // パスクエリ用
   SegmentTree<long long> seg_tree_path_node_weight(path_time_node_weight, sum_oper_, unit_sum);
   SegmentTree<long long> seg_tree_path_edge_weight(path_time_edge_weight, sum_oper_, unit_sum);

   seg_tree_path_node_weight_ = seg_tree_path_node_weight;
   seg_tree_path_edge_weight_ = seg_tree_path_edge_weight;

   // LCA用
   long long unit_min = numeric_limits<long long>::max();
   SegmentTree<long long> seg_tree_lca_depth_node(lca_depth_node_index, min_oper_, unit_min);
   SparseTable<long long> sp_table_lca_depth_node(lca_depth_node_index, min_oper_);
   sp_table_lca_depth_node_ = sp_table_lca_depth_node;
}

pair<int, int> EulerTour::GetInOutTime(int node) const {
   assert(1 <= node && node <= N_);
   assert(build_);

   int in_time = node_in_time_[node];
   int out_time = node_out_time_[node];

   return {in_time, out_time};
}

int EulerTour::GetSubTreeSize(const int node) const {
   auto [in_time, out_time] = GetInOutTime(node);
   int subtree_edge_count = (out_time - in_time) / 2;  // 辺を2回通る
   return subtree_edge_count + 1;
}

long long EulerTour::SubTreeQuery(const int node) const {
   auto [in_time, out_time] = GetInOutTime(node);

   long long ans = 0;

   // [in_time, out_time]区間のノードの和
   ans += seg_tree_subtree_node_weight_.query(in_time, out_time + 1);

   // 子への辺の和: (in_time, out_time]のエッジの和
   ans += seg_tree_subtree_edge_weight_.query(in_time + 1, out_time + 1);

   return ans;
}

long long EulerTour::PathQuery(const int node) const {
   long long ans = 0;

   auto [in_time, out_time] = GetInOutTime(node);

   // [0, out_time]区間のノードの和
   ans += seg_tree_path_node_weight_.query(0, out_time + 1);

   // 子への辺の和: (0, out_time]のエッジの和
   ans += seg_tree_path_edge_weight_.query(0 + 1, out_time + 1);

   return ans;
}

long long EulerTour::GetDepthNodeIndex(int depth, int node) const {
   if (depth < 0) return numeric_limits<long long>::max();

   long long index = depth;
   index <<= 30;
   index |= node;

   return index;
}

pair<int, int> EulerTour::GetDepthNode(long long index) const {
   static constexpr long long kMask = (1LL << 30) - 1;
   long long node = index & kMask;
   long long depth = index >> 30;

   return {depth, node};
}

int EulerTour::GetDepth(const int node) const {
   auto [in_time, out_time] = GetInOutTime(node);

   long long depth_node_index = sp_table_lca_depth_node_.query(in_time, in_time + 1);
   auto [depth, node_no] = GetDepthNode(depth_node_index);

   return depth;
}

int EulerTour::GetLCA(const int node_1, const int node_2) const {
   auto [in_time_1, out_time_1] = GetInOutTime(node_1);
   auto [in_time_2, out_time_2] = GetInOutTime(node_2);

   int in_time = min(in_time_1, in_time_2);
   int out_time = max(out_time_1, out_time_2);

   // node_1からnode_2へDFS順に移動する際に最も根に近い頂点を求める
   long long lca_depth_node_index = sp_table_lca_depth_node_.query(in_time, out_time + 1);
   auto [lca_depth, lca_node] = GetDepthNode(lca_depth_node_index);

   return lca_node;
}

long long EulerTour::PathQuery(const int node_1, const int node_2) const {
   long long ans = 0;

   // root -> node_1, root -> node_2
   ans += PathQuery(node_1);
   ans += PathQuery(node_2);

   // root -> lcaの辺コスト, ノードの重みが2回足されているので引く
   int lca_node = GetLCA(node_1, node_2);
   ans -= PathQuery(lca_node) * 2;

   // lcaノードの重みを戻す
   auto [lca_in_time, lca_out_time] = GetInOutTime(lca_node);
   ans += seg_tree_subtree_node_weight_.query(lca_in_time, lca_in_time + 1);

   return ans;
}

void EulerTour::UpdateNodeWeight(const int node, const long long weight) {
   auto [in_time, out_time] = GetInOutTime(node);

   node_weight_[node] = weight;

   // 部分木クエリ用は行きがけ時に記録
   seg_tree_subtree_node_weight_.update(in_time, weight);

   seg_tree_path_node_weight_.update(in_time, weight);
   seg_tree_path_node_weight_.update(out_time + 1, -weight);
}

void EulerTour::UpdateEdgeWeight(int edge_index, const long long weight) {
   auto [node_1, node_2, p_weight] = edge_list_[edge_index];
   get<2>(edge_list_[edge_index]) = weight;

   auto [in_time_1, out_time_1] = GetInOutTime(node_1);
   auto [in_time_2, out_time_2] = GetInOutTime(node_2);

   // 辺の重みは子ノードのin/out時刻に設定される
   int in_time = max(in_time_1, in_time_2);
   int out_time = min(out_time_1, out_time_2);

   // 部分木クエリ用は行きがけ時に記録
   seg_tree_subtree_edge_weight_.update(in_time, weight);

   // パスクエリ用は行きがけ時と帰りがけ時の抜けた後に記録
   seg_tree_path_edge_weight_.update(in_time, weight);
   seg_tree_path_edge_weight_.update(out_time + 1, -weight);
}

long long EulerTour::GetNodeWeight(const int node) const {
   return node_weight_[node];
}

long long EulerTour::GetEdgeWeight(const int edge_index) const {
   return get<2>(edge_list_[edge_index]);
}

bool EulerTour::InSubtree(const int sub_tree_root, const int node) const {
   auto [sub_in_time, sub_out_time] = GetInOutTime(sub_tree_root);
   auto [node_in_time, node_out_time] = GetInOutTime(node);

   return (sub_in_time <= node_in_time) && (node_out_time <= sub_out_time);
}

int EulerTour::GetPathLength(const int node_1, const int node_2) const {
   int ans = 0;

   ans += GetDepth(node_1);
   ans += GetDepth(node_2);

   int lca_node = GetLCA(node_1, node_2);
   ans -= GetDepth(lca_node) * 2;

   return ans;
}

// [End] Euler Tour

// [Start] Strongly Connected Components
// [Prefix] g-scc-class
// 参考: Technical Note - 強連結成分（SCC）(https://hkawabata.github.io/technical-note/note/Algorithm/graph/scc.html)
class StronglyConnectedComponents {
  public:
   using WeightedEdge = pair<int, long long>;

   // @param N 頂点数
   // @param adj_list 隣接リスト(重みあり)
   // 計算量: O(N+E)
   // [Verified] N <= 1 * 10^4, E <= 3 * 10^4, Q <= 1 * 10^5: Strongly Connected Components(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_C&lang=ja)
   // [Verified] N <= 5 * 10^5, E <= 5 * 10^5: Strongly Connected Components(https://judge.yosupo.jp/problem/scc)
   StronglyConnectedComponents(const vector<vector<WeightedEdge>>& adj_list);

   // @param N 頂点数
   // @param adj_list 隣接リスト(重みなし)
   // 計算量: O(N+E)
   StronglyConnectedComponents(const vector<vector<int>>& adj_list);

   // 強連結成分数を返す
   // 計算量: O(1)
   int GetSCCSize() const;

   // 強連結成分のグラフを返す
   // @note DAGかつトポロジカルソート済
   // 計算量: O(1)
   vector<vector<int>> GetSCCGraph() const;

   // 頂点nodeが所属する強連結成分No(1-indexed)を返す
   // 計算量: O(1)
   int GetNodeSCCNo(int node) const;

   // 「SCC番号 -> SCCに所属するノード番号リスト」の一覧を返す
   // 計算量: O(1)
   vector<vector<int>> GetSCCNodeGroup() const;

  protected:
   // 強連結成分を求める
   void Build();

   int N_;  // 頂点数

   vector<vector<WeightedEdge>> org_adj_list_;  // グラフの隣接リスト
   vector<vector<WeightedEdge>> rev_adj_list_;  // 逆辺グラフの隣接リスト

   vector<int> node_to_scc_no_;          // nodeが所属する強連結成分番号
   vector<vector<int>> scc_adj_list_;    // 強連結成分の隣接リスト(DAGになっている)
   vector<vector<int>> scc_node_group_;  // 強連結成分 -> 所属するノード番号リスト
};

StronglyConnectedComponents::StronglyConnectedComponents(const vector<vector<WeightedEdge>>& adj_list)
    : N_(adj_list.size() - 1), org_adj_list_(adj_list), rev_adj_list_(N_ + 1) {
   assert(!adj_list.empty());

   for (int from = 1; from <= N_; from++) {
      for (const auto& [to, weight] : adj_list[from]) {
         rev_adj_list_[to].emplace_back(from, weight);
      }
   }

   Build();
}

StronglyConnectedComponents::StronglyConnectedComponents(const vector<vector<int>>& adj_list)
    : N_(adj_list.size() - 1), org_adj_list_(N_ + 1), rev_adj_list_(N_ + 1) {
   assert(!adj_list.empty());

   for (int from = 1; from <= N_; from++) {
      for (const auto& to : adj_list[from]) {
         org_adj_list_[from].emplace_back(to, 0);
         rev_adj_list_[to].emplace_back(from, 0);
      }
   }

   Build();
}

void StronglyConnectedComponents::Build() {
   // 帰りがけ順の番号を記録する
   vector<int> node_post_order(N_ + 1, -1);     // node -> 帰りがけ順の番号
   vector<int> post_order_to_node(N_ + 1, -1);  // 帰りがけ順 -> node

   int post_order = 0;
   vector<bool> post_order_visited(N_ + 1, false);

   auto post_ordering = [&](auto post_ordering, int node) -> void {
      assert(!post_order_visited[node]);
      post_order_visited[node] = true;

      for (const auto& [n_node, weight] : org_adj_list_[node]) {
         if (post_order_visited[n_node]) continue;
         post_ordering(post_ordering, n_node);
      }

      post_order++;
      node_post_order[node] = post_order;
      post_order_to_node[post_order] = node;
   };

   for (int node = 1; node <= N_; node++) {
      if (post_order_visited[node]) continue;
      post_ordering(post_ordering, node);
   }

   // 強連結成分を列挙する
   node_to_scc_no_.resize(N_ + 1, -1);
   int scc_no = 0;

   scc_adj_list_.resize(N_ + 1);

   auto scc_numbering = [&](auto scc_numbering, int node) -> void {
      assert(node_to_scc_no_[node] == -1);
      node_to_scc_no_[node] = scc_no;

      for (const auto& [n_node, weight] : rev_adj_list_[node]) {
         if (node_to_scc_no_[n_node] != -1) {
            if (node_to_scc_no_[n_node] != scc_no) {
               // 逆辺グラフで先に作られた成分へ移動できる
               // -> 元のグラフで先に作られたSCCから今のSCCへ移動可能
               // ただし、多重辺になる可能性があるため後で取り除く
               int from_scc = node_to_scc_no_[n_node];
               scc_adj_list_[from_scc].emplace_back(scc_no);
            }

            continue;
         }

         scc_numbering(scc_numbering, n_node);
      }
   };

   for (int post_order = N_; post_order >= 1; post_order--) {
      int node = post_order_to_node[post_order];

      if (node_to_scc_no_[node] != -1) continue;

      scc_no++;
      scc_numbering(scc_numbering, node);
   }

   scc_adj_list_.resize(scc_no + 1);

   // 多重辺になる可能性があるため除去する
   for (int i = 1; i <= scc_no; i++) {
      // (強連結成分は昇順で記録されているためソート不要)
      scc_adj_list_[i].erase(unique(scc_adj_list_[i].begin(), scc_adj_list_[i].end()), scc_adj_list_[i].end());
   }

   scc_node_group_.resize(scc_no + 1);

   for (int node = 1; node <= N_; node++) {
      int node_scc_no = GetNodeSCCNo(node);

      scc_node_group_[node_scc_no].emplace_back(node);
   }
}

int StronglyConnectedComponents::GetSCCSize() const {
   assert(!scc_adj_list_.empty());
   int scc_count = scc_adj_list_.size() - 1;
   return scc_count;
}

int StronglyConnectedComponents::GetNodeSCCNo(int node) const {
   assert(1 <= node && node <= N_);
   return node_to_scc_no_[node];
}

vector<vector<int>> StronglyConnectedComponents::GetSCCGraph() const {
   return scc_adj_list_;
}

vector<vector<int>> StronglyConnectedComponents::GetSCCNodeGroup() const {
   return scc_node_group_;
}

// [End] Strongly Connected Components

void BipartiteCheckDFS() {
   int N = 1;
   vector<vector<int>> adj_list;

   // clang-format off
   // [Start] Bipartite Check(DFS)
   // [Prefix] g-bipartite-dfs-inline
   // cur_vを含む連結成分が二部グラフかを判定する。
   // 計算量: O(N + E)
   // node_color配列に
   //    -1: 未訪問, 0: 赤, 1: 青
   // を対応させる
   // [Verified] N, E <= 2 x 10^5: ABC327「D - Good Tuple Problem」(https://atcoder.jp/contests/abc327/tasks/abc327_d)
   // 参考: https://qiita.com/drken/items/a803d4fc4a727e02f7ba#4-3-%E4%BA%8C%E9%83%A8%E3%82%B0%E3%83%A9%E3%83%95%E5%88%A4%E5%AE%9A
   vector<int> node_color(N + 1, -1);

   auto dfs = [&](auto dfs, int cur_v, int p_color) -> bool {
      // cur_v:
      // p_color: 親の色
      if (node_color[cur_v] != -1) {
         // すでに訪問済み
         return true;
      }

      int color = 1 - p_color;
      node_color[cur_v] = color;

      for (auto n_v : adj_list[cur_v]) {
         if (node_color[n_v] != -1) {
            if (node_color[n_v] == color) {
               // 同じ色が隣接していたら二部グラフではない
               return false;
            }
            continue;
         }

         auto res = dfs(dfs, n_v, color);

         if (!res) {
            return false;
         }
      }

      return true;
   };
   // [End] Bipartite Check(DFS)
   // clang-format on
}

// [Start] Bipartite Check(UnionFind)
// [Prefix] g-bipartite-uf-func
// 与えられたグラフが二部グラフかを判定する。
// 計算量: O(N + E alpha(N))
// 依存ライブラリ: UnionFind
// [Verified] N, E <= 2 x 10^5: ABC327「D - Good Tuple Problem」(https://atcoder.jp/contests/abc327/tasks/abc327_d)
// 参考: https://noshi91.hatenablog.com/entry/2018/04/17/183132
bool BipartiteCheckUF(int N, const vector<pair<int, int>>& edge_list) {
   // 頂点倍化グラフ(V -> V_a, V_b)を考える
   UnionFind uf(2 * N + 1);

   for (auto [u, v] : edge_list) {
      // 枝(u, v)に対して(a_v, b_u), (a_u, b_v)を結ぶ
      int a_u = u, a_v = v;
      int b_u = u + N, b_v = v + N;

      uf.Unite(a_u, b_v);
      uf.Unite(a_v, b_u);
   }

   for (int v = 1; v <= N; v++) {
      if (uf.IsSameGroup(v, v + N)) {
         // vが同じグループに属する -> 二部グラフではない
         return false;
      }
   }

   return true;
}
// [End] Bipartite Check(UnionFind)

// [Start] Doubling(Tree)
// [Prefix] g-doubling-tree-class
// Doublingで木構造に対する処理を行う
// 参考: https://ikatakos.com/pot/programming_algorithm/graph_theory/lowest_common_ancestor
class DoublingOnTree {
  public:
   // 重みなし版
   DoublingOnTree(const int N, const int root_node, const vector<pair<int, int>>& edge_list);

   // 重みあり版
   DoublingOnTree(const int N, const int root_node, const vector<tuple<int, int, long long>>& edge_list);

   // LCAを求める
   // 計算量: 前処理 O(N log N), クエリ O(log N)
   // [Verified] N, Q <= 5 * 10^5: Lowest Common Ancestor(https://judge.yosupo.jp/problem/lca)
   int GetLCA(int u, int v);

   // 2頂点間の辺の重みの合計を求める
   // 計算量: 前処理 O(N log N), クエリ O(log N)
   long long CalcPathWeight(const int u, const int v);

   // 2頂点間のパスの長さ(辺の数)を求める
   // 計算量: 前処理 O(N log N), クエリ O(log N)
   int GetPathLength(const int u, const int v);

   // 2頂点間のパスの最大重みを求める
   // 計算量: 前処理 O(N log N), クエリ O(log N)
   // [Verified] N <= 2 x 10^5, Q <= 2 x 10^5: ABC235「E - MST + 1」(https://atcoder.jp/contests/abc235/tasks/abc235_e)
   long long CalcMaxWeightOnPath(int u, int v);

   // 2頂点間のパスの最小重みを求める
   long long CalcMinWeightOnPath(int u, int v);

  protected:
   // LCA用の前処理を行う
   // 計算量: O(N log N)
   void InitLCA();

   // 最大重み用の前処理を行う
   // 計算量: O(N log N)
   void InitMaxWeight();

   // 最小重み用の前処理を行う
   // 計算量: O(N log N)
   void InitMinWeight();

   int N_;          // 頂点数
   int root_node_;  // 根ノード

   vector<vector<pair<int, long long>>> adj_list_;  // 隣接リスト
   vector<tuple<int, int, long long>> edge_list_;   // 辺リスト

   vector<int> depth_;           // 各ノードの根からの深さ
   vector<long long> distance_;  // 各ノードの根からの距離

   vector<vector<int>> ancestors_;  // ancestors_[k][i]: ノードiの2^k個上の祖先

   vector<vector<long long>> max_weights_;  // max_weights_[k][i]: ノードiの2^k個上までの祖先までのpath上での最大重み
   vector<vector<long long>> min_weights_;  // min_weights_[k][i]: ノードiの2^k個上までの祖先までのpath上での最小重み

   bool init_lca_;         // LCA用の初期化フラグ
   bool init_max_weight_;  // 最大重み用の初期化フラグ
   bool init_min_weight_;  // 最小重み用の初期化フラグ
};

DoublingOnTree::DoublingOnTree(const int N, const int root_node, const vector<pair<int, int>>& edge_list)
    : N_(N), root_node_(root_node), init_lca_(false), init_max_weight_(false), init_min_weight_(false) {
   adj_list_.resize(N_ + 1);
   int M = edge_list.size();

   for (int i = 0; i < M; i++) {
      auto [u, v] = edge_list[i];

      adj_list_[u].emplace_back(v, 1);
      adj_list_[v].emplace_back(u, 1);

      edge_list_.emplace_back(u, v, 1);
   }
}

DoublingOnTree::DoublingOnTree(const int N, const int root_node, const vector<tuple<int, int, long long>>& edge_list)
    : N_(N), root_node_(root_node), init_lca_(false), init_max_weight_(false), init_min_weight_(false) {
   adj_list_.resize(N_ + 1);
   int M = edge_list.size();

   for (int i = 0; i < M; i++) {
      auto [u, v, w] = edge_list[i];

      adj_list_[u].emplace_back(v, w);
      adj_list_[v].emplace_back(u, w);

      edge_list_.emplace_back(u, v, w);
   }
}

void DoublingOnTree::InitLCA() {
   if (init_lca_) return;
   init_lca_ = true;

   depth_.resize(N_ + 1, 0);
   distance_.resize(N_ + 1, 0);

   vector<int> direct_ancestors(N_ + 1, root_node_);  // 直近の祖先ノード(根の親は根にする)

   auto dfs = [&](auto dfs, int v, int p) -> void {
      for (auto [n_v, w] : adj_list_[v]) {
         if (n_v == p) continue;

         depth_[n_v] = depth_[v] + 1;
         distance_[n_v] = distance_[v] + w;
         direct_ancestors[n_v] = v;

         dfs(dfs, n_v, v);
      }
   };

   dfs(dfs, root_node_, -1);

   int max_depth = -1;

   for (int v = 1; v <= N_; v++) {
      max_depth = max(max_depth, depth_[v]);
   }

   int d = 1;

   ancestors_.emplace_back(direct_ancestors);

   while (d < max_depth) {
      vector<int> next_ancestors(N_ + 1, -1);
      const auto& prev_ancestors = ancestors_.back();

      for (int v = 1; v <= N_; v++) {
         next_ancestors[v] = prev_ancestors[prev_ancestors[v]];
      }

      ancestors_.emplace_back(next_ancestors);
      d *= 2;
   }
}

void DoublingOnTree::InitMaxWeight() {
   if (init_max_weight_) return;
   init_max_weight_ = true;

   InitLCA();

   int max_depth = -1;

   for (int v = 1; v <= N_; v++) {
      max_depth = max(max_depth, depth_[v]);
   }

   int d = 1;

   // 1つ上の祖先へのpathの重み
   vector<long long> direct_max_weights(N_ + 1, 0);

   for (int v = 1; v <= N_; v++) {
      auto p_v = ancestors_[0][v];
      direct_max_weights[v] = distance_[v] - distance_[p_v];
   }

   max_weights_.emplace_back(direct_max_weights);

   while (d < max_depth) {
      vector<long long> next_max_weights(N_ + 1, 0);
      const auto& prev_max_weights = max_weights_.back();
      int k = (int)max_weights_.size() - 1;

      for (int v = 1; v <= N_; v++) {
         int p_v = ancestors_[k][v];
         next_max_weights[v] = max(prev_max_weights[v], prev_max_weights[p_v]);
      }

      max_weights_.emplace_back(next_max_weights);
      d *= 2;
   }
}

void DoublingOnTree::InitMinWeight() {
   if (init_min_weight_) return;
   init_min_weight_ = true;

   InitLCA();

   int max_depth = -1;

   for (int v = 1; v <= N_; v++) {
      max_depth = max(max_depth, depth_[v]);
   }

   int d = 1;

   // 1つ上の祖先へのpathの重み
   vector<long long> direct_min_weights(N_ + 1, 0);

   for (int v = 1; v <= N_; v++) {
      auto p_v = ancestors_[0][v];
      direct_min_weights[v] = distance_[v] - distance_[p_v];
   }

   min_weights_.emplace_back(direct_min_weights);

   while (d < max_depth) {
      vector<long long> next_min_weights(N_ + 1, 0);
      const auto& prev_min_weights = min_weights_.back();
      int k = (int)min_weights_.size() - 1;

      for (int v = 1; v <= N_; v++) {
         int p_v = ancestors_[k][v];
         next_min_weights[v] = min(next_min_weights[v], prev_min_weights[p_v]);
      }

      min_weights_.emplace_back(next_min_weights);
      d *= 2;
   }
}

int DoublingOnTree::GetLCA(int u, int v) {
   InitLCA();

   assert(1 <= u && u <= N_);
   assert(1 <= v && v <= N_);

   int depth_u = depth_[u];
   int depth_v = depth_[v];

   if (depth_u > depth_v) {
      swap(u, v);
      swap(depth_u, depth_v);
   }

   // uとvの深さを揃える(vの方が深いのでuと同じ高さになるまでさかのぼる)
   int diff = depth_v - depth_u;

   for (int i = 0; i < (int)ancestors_.size(); i++) {
      if (diff & (1 << i)) {
         v = ancestors_[i][v];
      }
   }

   if (u == v) return u;

   // (u, v)から見て2^k上の祖先が異なれば高さを2^k上げることを繰り返す
   // k=0まで行うと2^0=1つ上が共通祖先になっている
   for (int k = (int)ancestors_.size() - 1; k >= 0; k--) {
      if (ancestors_[k][u] != ancestors_[k][v]) {
         u = ancestors_[k][u];
         v = ancestors_[k][v];
      }
   }

   assert(ancestors_[0][u] == ancestors_[0][v]);
   return ancestors_[0][u];
}

long long DoublingOnTree::CalcMaxWeightOnPath(int u, int v) {
   InitMaxWeight();
   int lca = GetLCA(u, v);

   auto calc_max_weight_to_lca = [&](int node) -> long long {
      int depth_diff = depth_[node] - depth_[lca];
      long long max_weight = 0;

      for (int i = 0; i < (int)ancestors_.size(); i++) {
         if (depth_diff & (1 << i)) {
            max_weight = max(max_weight, max_weights_[i][node]);
            node = ancestors_[i][node];
         }
      }

      return max_weight;
   };

   long long max_from_u = calc_max_weight_to_lca(u);
   long long max_from_v = calc_max_weight_to_lca(v);

   return max(max_from_u, max_from_v);
}

long long DoublingOnTree::CalcMinWeightOnPath(int u, int v) {
   InitMinWeight();
   int lca = GetLCA(u, v);

   auto calc_min_weight_to_lca = [&](int node) -> long long {
      int depth_diff = depth_[node] - depth_[lca];
      long long min_weight = 0;

      for (int i = 0; i < (int)ancestors_.size(); i++) {
         if (depth_diff & (1 << i)) {
            min_weight = min(min_weight, min_weights_[i][node]);
            node = ancestors_[i][node];
         }
      }

      return min_weight;
   };

   long long min_from_u = calc_min_weight_to_lca(u);
   long long min_from_v = calc_min_weight_to_lca(v);

   return min(min_from_u, min_from_v);
}

int DoublingOnTree::GetPathLength(const int u, const int v) {
   int lca = GetLCA(u, v);
   auto length = depth_[u] + depth_[v] - 2 * depth_[lca];

   return length;
}

long long DoublingOnTree::CalcPathWeight(const int u, const int v) {
   int lca = GetLCA(u, v);
   auto dist = distance_[u] + distance_[v] - 2 * distance_[lca];

   return dist;
}

// [End] Doubling(Tree)

// [Start] Prufer sequence
// [Prefix] g-prufer-tree-func

// 木からPrufer sequenceを求める
// @param N 頂点数
// @param edge_list 辺リスト(1-indexed)
// @retval Prufer sequence(長さN-2)
// 計算量: O(N log N)
vector<int> GetPruferSequence(int N, const vector<pair<int, int>>& edge_list) {
   assert(N >= 2);
   assert((int)edge_list.size() == N - 1);

   vector<set<int>> adj_list(N + 1);
   vector<int> degree(N + 1, 0);

   for (auto [u, v] : edge_list) {
      degree[u]++;
      degree[v]++;

      adj_list[u].emplace(v);
      adj_list[v].emplace(u);
   }

   set<int> leaf_nodes;

   for (int v = 1; v <= N; v++) {
      if (degree[v] == 1) {
         leaf_nodes.emplace(v);
      }
   }

   vector<int> prufer_seq(N - 2, 0);

   for (int i = 0; i < N - 2; i++) {
      assert(!leaf_nodes.empty());
      auto leaf_node = *leaf_nodes.begin();

      assert(adj_list[leaf_node].size() == 1);
      int next_node = *(adj_list[leaf_node].begin());

      prufer_seq[i] = next_node;

      degree[next_node]--;
      adj_list[next_node].erase(leaf_node);

      leaf_nodes.erase(leaf_node);

      if (degree[next_node] == 1) {
         leaf_nodes.emplace(next_node);
      }
   }

   return prufer_seq;
}

// Prufer sequenceから木を求める
// @param N 頂点数
// @param prufer_sequence Prufer列(頂点番号は1-indexed)
// @retval 辺(頂点pair)リスト(1-indexed)
// 計算量: O(N log N)
vector<pair<int, int>> GetTree(int N, const vector<int>& prufer_sequence) {
   assert(N >= 2);
   assert((int)prufer_sequence.size() == N - 2);

   vector<int> degree(N + 1, 1);

   for (auto v : prufer_sequence) {
      degree[v]++;
   }

   set<int> leaf_nodes;

   for (int v = 1; v <= N; v++) {
      if (degree[v] == 1) {
         leaf_nodes.emplace(v);
      }
   }

   vector<pair<int, int>> edge_list;

   for (auto v : prufer_sequence) {
      assert(1 <= v && v <= N);

      assert(!leaf_nodes.empty());
      auto leaf_node = *leaf_nodes.begin();

      edge_list.emplace_back(leaf_node, v);

      degree[leaf_node]--;
      degree[v]--;

      leaf_nodes.erase(leaf_node);

      if (degree[v] == 1) {
         leaf_nodes.emplace(v);
      }
   }

   assert(leaf_nodes.size() == 2);
   auto leaf_node_1 = *leaf_nodes.begin();
   auto leaf_node_2 = *leaf_nodes.rbegin();

   edge_list.emplace_back(leaf_node_1, leaf_node_2);

   return edge_list;
}

// ラベル付き木を列挙する
// @param N 頂点数
// @retval ラベル付き木の辺(頂点ペア)のリストのリスト
// @note ラベル付き木の数はN^(N-2)個, N=10で10^8になるので注意
// [Verified] N<=8, ABC_328「E - Modulo MST」(https://atcoder.jp/contests/abc328/tasks/abc328_e)
vector<vector<pair<int, int>>> EnumerateTrees(int N) {
   if (N <= 1) {
      return vector<vector<pair<int, int>>>({{}});
   }

   assert(N <= 10);  // N > 10だと計算量が多すぎるためREにする

   long long tree_count = 1;

   for (int i = 0; i < N - 2; i++) {
      tree_count *= N;
   }

   vector<vector<pair<int, int>>> tree_list;
   tree_list.reserve(tree_count);

   auto calc_prufer_seq = [&](int tree_ind) -> vector<int> {
      vector<int> prufer_seq(N - 2, 0);

      for (int i = 0; i < N - 2; i++) {
         prufer_seq[i] = (tree_ind % N) + 1;
         tree_ind /= N;
      }

      return prufer_seq;
   };

   for (int tree_ind = 0; tree_ind < tree_count; tree_ind++) {
      auto prufer_seq = calc_prufer_seq(tree_ind);
      auto edge_list = GetTree(N, prufer_seq);

      tree_list.emplace_back(edge_list);
   }

   return tree_list;
}
// [End] Prufer sequence
