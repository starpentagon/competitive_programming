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

   // 2ノード間の辺の重みの合計を求める
   // 計算量: 前処理 O(N log N), クエリ O(log N)
   long long CalcPathWeight(const int u, const int v);

   // 2ノード間のパスの長さ(辺の数)を求める
   // 計算量: 前処理 O(N log N), クエリ O(log N)
   int GetPathLength(const int u, const int v);

   // 2ノード間のパスの最大重みを求める
   // 計算量: 前処理 O(N log N), クエリ O(log N)
   // [Verified] N <= 2 x 10^5, Q <= 2 x 10^5: ABC235「E - MST + 1」(https://atcoder.jp/contests/abc235/tasks/abc235_e)
   long long CalcMaxWeightOnPath(int u, int v);

   // 2ノード間のパスの最小重みを求める
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

// UnionFindで素集合を管理するクラス
// ノードIDは1-indexed
class UnionFind {
  public:
   UnionFind(const size_t N);

   // node_1とnode_2が所属する集合を結合する(Union by size)
   // 計算量: O(almost 1)
   void Unite(size_t node_1, size_t node_2);

   // node_1とnode_2が同じ集合に入っているか
   // 計算量: O(almost 1)
   bool IsSameGroup(size_t node_1, size_t node_2) const;

   // nodeが所属する集合のサイズを返す
   // 計算量: O(almost 1)
   size_t size(size_t node) const;

   // 各グループを返す(親ノードの番号 -> 子のリスト)
   map<size_t, vector<size_t>> EnumGroup() const;

   // nodeの親のnode_idを返す
   size_t root(size_t node) const;

  private:
   // ノードの数
   size_t N_;

   // parent_node_id_[i]: 親ノードのノードID, rootの場合はparent_node_id_[i] ==
   // iになる
   mutable vector<size_t> parent_node_id_;

   // tree_size_[i]: ルートノードiに含まれる集合のサイズ
   vector<size_t> tree_size_;
};

UnionFind::UnionFind(const size_t N)
    : N_(N), parent_node_id_(N + 1), tree_size_(N + 1, 1) {
   // 全ノードをrootで初期化する
   for (size_t i = 0; i <= N; i++) {
      parent_node_id_[i] = i;
   }
}

size_t UnionFind::root(size_t node) const {
   assert(1 <= node && node <= N_);

   // ルートノード以外のノードを記録し直接、親ノードをルートノードにつなぎ変える(経路圧縮)
   vector<size_t> internal_nodes;

   while (parent_node_id_[node] != node) {
      internal_nodes.push_back(node);
      node = parent_node_id_[node];
   }

   for (auto n : internal_nodes) {
      parent_node_id_[n] = node;
   }

   return node;
}

bool UnionFind::IsSameGroup(size_t node_1, size_t node_2) const {
   auto parent_1 = root(node_1);
   auto parent_2 = root(node_2);

   return parent_1 == parent_2;
}

void UnionFind::Unite(size_t node_1, size_t node_2) {
   if (IsSameGroup(node_1, node_2)) {
      // すでに同じ木の場合は何もしない
      return;
   }

   // サイズの小さい方を大きいにつなぐ
   auto size_1 = size(node_1);
   auto size_2 = size(node_2);

   size_t union_from = node_1, union_to = node_2;

   if (size_1 > size_2) {
      union_from = node_2;
      union_to = node_1;
   }
   auto parent_from = root(union_from);
   auto parent_to = root(union_to);

   parent_node_id_[parent_from] = parent_to;
   tree_size_[parent_to] = tree_size_[parent_to] + tree_size_[parent_from];
}

size_t UnionFind::size(size_t node) const {
   auto parent = root(node);
   return tree_size_[parent];
}

map<size_t, vector<size_t>> UnionFind::EnumGroup() const {
   map<size_t, vector<size_t>> group;

   for (size_t i = 1; i <= N_; i++) {
      auto parent = root(i);
      group[parent].push_back(i);
   }

   return group;
}

int main() {
   int N, M, Q;
   cin >> N >> M >> Q;

   vector<tuple<ll, int, int>> all_edge_list;

   rep(i, M) {
      ll a, b, c;
      cin >> a >> b >> c;

      all_edge_list.emplace_back(c, a, b);
   }

   sort(ALL(all_edge_list));
   UnionFind uf(N);

   vector<tuple<int, int, ll>> mst_edge_list;

   for (auto [w, u, v] : all_edge_list) {
      if (uf.IsSameGroup(u, v)) continue;
      uf.Unite(u, v);
      mst_edge_list.emplace_back(u, v, w);
   }

   debug(mst_edge_list);
   DoublingOnTree doubling(N, 1, mst_edge_list);

   rep(q, Q) {
      ll u, v, w;
      cin >> u >> v >> w;

      ll max_weight = doubling.CalcMaxWeightOnPath(u, v);

      if (w < max_weight) {
         cout << "Yes" << endl;
      } else {
         cout << "No" << endl;
      }
   }

   return 0;
}
