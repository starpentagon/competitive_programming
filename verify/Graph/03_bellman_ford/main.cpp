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

// ベルマン・フォード法で単一始点最短路を求める
// @note 負の重みが存在しても正しく動く
// @note 負の閉路があれば検知可能
// @retval true 負の閉路が存在, false 負の閉路が存在しない
// @note 負の閉路が存在する場合、重みがマイナス無限大になるノードをDetectNegtiveInfNode()で特定する必要がある
// @note 計算量: O(E * N)
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

int main() {
   int N, M, r;
   cin >> N >> M >> r;

   r++;

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

   auto [negative, min_dist_tbl] = ShortestPathBellmanFord(adj_list, r);

   if (negative) {
      cout << "NEGATIVE CYCLE" << endl;
      return 0;
   }

   for (int i = 1; i <= N; i++) {
      auto ans = min_dist_tbl[i];

      if (ans == numeric_limits<ll>::max()) {
         cout << "INF" << endl;
      } else {
         cout << ans << endl;
      }
   }

   return 0;
}