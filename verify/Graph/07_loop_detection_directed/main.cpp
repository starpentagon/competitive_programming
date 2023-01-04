#include <algorithm>
#include <iostream>
#include <vector>
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

// 有向グラフで閉路を求める
// @retval 閉路のリスト。閉路はmap[node]: to_nodeのマップで表現
// @note 連結成分ごとに閉路があればそのうち1つを返す
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

int main() {
   int N, M;
   cin >> N >> M;

   // N: nodes, M: edges
   using Edge = pair<int, int>;
   vector<Edge> edge_index_list;

   for (int i = 0; i < M; i++) {
      int u, v;
      cin >> u >> v;

      u++;
      v++;

      edge_index_list.emplace_back(u, v);
   }

   auto loop_list = FindLoopDirected(N, edge_index_list);

   if (loop_list.empty()) {
      cout << -1 << endl;
   } else {
      cout << loop_list[0].size() << endl;
      for (auto e : loop_list[0]) {
         cout << e << endl;
      }
   }

   return 0;
}