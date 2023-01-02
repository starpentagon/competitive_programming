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
// ワーシャル・フロイド法で全点対間最短路を求める
// @note 負の重みが存在しても正しく動く
// @note 負の閉路があれば検知可能
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

int main() {
   int N, M;
   cin >> N >> M;

   // N: nodes, M: edges
   using Edge = pair<int, long long>;  // to, weight
   vector<vector<Edge>> adj_list(N + 1, vector<Edge>());

   for (int i = 0; i < M; i++) {
      int u, v;
      cin >> u >> v;

      u++;
      v++;

      long long w;
      cin >> w;

      adj_list[u].emplace_back(v, w);
   }

   auto [negative, min_dist_tbl] = AllShortestPathWarshallFloyd(adj_list);

   if (negative) {
      cout << "NEGATIVE CYCLE" << endl;
      return 0;
   }

   for (int i = 1; i <= N; i++) {
      for (int j = 1; j <= N; j++) {
         auto ans = min_dist_tbl[i][j];

         if (ans == numeric_limits<ll>::max()) {
            cout << "INF";
         } else {
            cout << ans;
         }

         if (j == N) {
            cout << endl;
         } else {
            cout << " ";
         }
      }
   }

   return 0;
}