#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <map>
#include <queue>

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

   rep(i, E) {
      if (edge_exist[i]) {
         all_edge_deleted = false;
         break;
      }
   }

   return {all_edge_deleted, sorted_list};
}

int main() {
   int N, M;
   cin >> N >> M;

   // N: nodes, M: edges
   vector<vector<int>> adj_list(N + 1, vector<int>());

   for (int i = 0; i < M; i++) {
      int u, v;
      cin >> u >> v;

      u++;
      v++;

      adj_list[u].emplace_back(v);
   }

   auto [flg, sorted] = TopologicalSort(adj_list);

   for (auto node : sorted) {
      cout << (node - 1) << endl;
   }

   return 0;
}