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

// BFSで最短路を求める
// 計算量: O(N+E)
// 非連結成分には numeric_limits<long long>::max() が設定される
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

int main() {
   int N;
   cin >> N;

   // N: nodes, M: edges
   vector<vector<int>> adj_list(N + 1, vector<int>());

   rep(i, N) {
      int u;
      cin >> u;

      int K;
      cin >> K;

      rep(k, K) {
         int v;
         cin >> v;

         adj_list[u].emplace_back(v);
      }
   }

   auto min_dist_tbl = ShortestPathBFS(N, adj_list, 1);

   rep(i, N) {
      auto dist = min_dist_tbl[i + 1];
      ll ans = -1;

      if (dist != numeric_limits<ll>::max()) {
         ans = dist;
      }

      cout << (i + 1) << ' ' << ans << endl;
   }

   return 0;
}