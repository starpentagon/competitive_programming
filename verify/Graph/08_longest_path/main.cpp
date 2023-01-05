#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <map>
#include <stack>
#include <unordered_map>

using namespace std;

// clang-format off
#define rep(i, n) for (int i = 0; (i) < (int)(n); (i)++)
 
template<class T> bool chmax(T &a, const T &b) {if(a<b) {a=b; return true;} return false; }
template<class T> bool chmin(T &a, const T &b) {if(a>b) {a=b; return true;} return false; }
 
template<class T> istream& operator>>(istream& is, vector<T>& vec){ rep(i, vec.size()) is >> vec[i]; return is;}
template<class T> ostream& operator<<(ostream& os, vector<T>& vec){ rep(i, vec.size()) os << vec[i] << (i+1==(int)vec.size() ? "" : " "); return os;}
// clang-format on

using ll = long long;
using ull = unsigned long long;

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

int main() {
   int N, M;
   cin >> N >> M;

   // N: nodes, M: edges
   vector<vector<int>> adj_list(N + 1);

   for (int i = 0; i < M; i++) {
      int u, v;
      cin >> u >> v;

      adj_list[u].emplace_back(v);
   }

   auto ans = LongestPath(adj_list);
   cout << ans << endl;

   return 0;
}