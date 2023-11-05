#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#define debug(...) debug_print::multi_print(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) ;
#endif

// clang-format off
#define rep(i, n) for (int i = 0; (i) < (int)(n); (i)++)
#define repp(i, s, n) for (int i = s; (i) < (int)(n); (i)++)

#define ALL(v) begin(v),end(v)
#define RALL(v) rbegin(v),rend(v)

template<class T> bool chmax(T &a, const T &b) {if(a<b) {a=b; return true;} return false; }
template<class T> bool chmin(T &a, const T &b) {if(a>b) {a=b; return true;} return false; }

template<class T> istream& operator>>(istream& is, vector<T>& vec){ rep(i, vec.size()) is >> vec[i]; return is;}
template<class T> ostream& operator<<(ostream& os, vector<T>& vec){ rep(i, vec.size()) os << vec[i] << (i+1==(int)vec.size() ? "" : " "); return os;}
// clang-format on

using ll = long long;
using ull = unsigned long long;

int main() {
   int N, M;
   cin >> N >> M;

   vector<int> al(M), bl(M);
   cin >> al >> bl;

   // N: nodes, M: edges
   vector<vector<int>> adj_list(N + 1, vector<int>());

   for (int i = 0; i < M; i++) {
      int u, v;
      u = al[i], v = bl[i];

      adj_list[u].emplace_back(v);
      adj_list[v].emplace_back(u);
   }

   // cur_vを含む連結成分が二部グラフかを判定する。
   // 計算量: O(N + E)
   // node_color配列に
   //    -1: 未訪問, 0: 赤, 1: 青
   // を対応させる
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

   bool ok = true;

   for (int v = 1; v <= N; v++) {
      auto check = dfs(dfs, v, 0);
      if (!check) {
         ok = false;
         break;
      }
   }

   if (ok) {
      cout << "Yes" << endl;
   } else {
      cout << "No" << endl;
   }

   return 0;
}
