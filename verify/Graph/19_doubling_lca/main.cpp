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

// Doublingで木構造に対する処理を行う
// Doublingで木構造に対する処理を行う
class DoublingOnTree {
  public:
   // 重みなし版
   DoublingOnTree(const int N, const int root_node, const vector<pair<int, int>>& edge_list);

   // 重みあり版
   DoublingOnTree(const int N, const int root_node, const vector<tuple<int, int, long long>>& edge_list);

   // LCAを求める
   // 計算量: 前処理 O(N log N), クエリ O(log N)
   int GetLCA(int u, int v) const;

  protected:
   // LCA用の前処理を行う
   // 計算量: O(N log N)
   void LCAInit();

   int N_;          // 頂点数
   int root_node_;  // 根ノード

   vector<vector<pair<int, long long>>> adj_list_;  // 隣接リスト
   vector<tuple<int, int, long long>> edge_list_;   // 辺リスト

   vector<int> depth_;     // 各ノードの根からの深さ
   vector<int> distance_;  // 各ノードの根からの距離

   vector<vector<int>> ancestors_;  // ancestors_[k][i]: ノードiの2^k個上の祖先
};

DoublingOnTree::DoublingOnTree(const int N, const int root_node, const vector<pair<int, int>>& edge_list)
    : N_(N), root_node_(root_node) {
   adj_list_.resize(N_ + 1);
   int M = edge_list.size();

   for (int i = 0; i < M; i++) {
      auto [u, v] = edge_list[i];

      adj_list_[u].emplace_back(v, 1);
      adj_list_[v].emplace_back(u, 1);

      edge_list_.emplace_back(u, v, 1);
   }

   LCAInit();
}

DoublingOnTree::DoublingOnTree(const int N, const int root_node, const vector<tuple<int, int, long long>>& edge_list)
    : N_(N), root_node_(root_node) {
   adj_list_.resize(N_ + 1);
   int M = edge_list.size();

   for (int i = 0; i < M; i++) {
      auto [u, v, w] = edge_list[i];

      adj_list_[u].emplace_back(v, w);
      adj_list_[v].emplace_back(u, w);

      edge_list_.emplace_back(u, v, w);
   }

   LCAInit();
}

void DoublingOnTree::LCAInit() {
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

int DoublingOnTree::GetLCA(int u, int v) const {
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

int main() {
   int N, Q;
   cin >> N >> Q;

   vector<pair<int, int>> edge_list;

   rep(i, N - 1) {
      int p;
      cin >> p;
      edge_list.emplace_back(i + 2, p + 1);
   }

   DoublingOnTree doubling(N, 1, edge_list);

   rep(q, Q) {
      int u, v;
      cin >> u >> v;
      u += 1;
      v += 1;

      int lca = doubling.GetLCA(u, v);
      cout << (lca - 1) << endl;
   }

   return 0;
}
