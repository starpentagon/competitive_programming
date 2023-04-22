#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#define debug(...) debug_print::multi_print(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) ;
#endif

// clang-format off
#define rep(i, n) for (int i = 0; (i) < (int)(n); (i)++)
 
template<class T> bool chmax(T &a, const T &b) {if(a<b) {a=b; return true;} return false; }
template<class T> bool chmin(T &a, const T &b) {if(a>b) {a=b; return true;} return false; }
 
template<class T> istream& operator>>(istream& is, vector<T>& vec){ rep(i, vec.size()) is >> vec[i]; return is;}
template<class T> ostream& operator<<(ostream& os, vector<T>& vec){ rep(i, vec.size()) os << vec[i] << (i+1==(int)vec.size() ? "" : " "); return os;}
// clang-format on

using ll = long long;
using ull = unsigned long long;

// セグメント木
// 計算量
// - 初期構築 O(N)
// - 一点更新 O(log N)
// - 添え字区間[a, b)の要素に対する演算結果 O(log N)
template <class X>
class SegmentTree {
  public:
   // @param val_list セグメント木を構築する配列
   // @param oper 二項演算子(最大、最小、XORなど)
   // @param unit 単位元(max: 0, min: INF, XOR: 0)
   SegmentTree(const vector<X>& val_list, function<X(X, X)> oper, const X unit);

   // i番目の要素の値を取得する(0-indexed)
   X get(size_t i) const {
      return data_[leaf_index(i)];
   }

   // i番目の要素の値を更新する
   void update(size_t i, X val);

   // i番目の要素の値に加算する
   void add(size_t i, X val) {
      auto v = get(i);
      v += val;

      update(i, v);
   }

   // 添え字区間[a, b)の要素に対する二項演算結果を返す
   X query(size_t a, size_t b) const {
      return sub_query(a, b,
                       0,          // ルートノード
                       0,          // 0番目以上
                       leaf_size_  // n番目未満
      );
   }

  private:
   // i番目の要素の値を設定する
   // @note 親ノードの更新は行わない。初期構築時に利用。
   void set(size_t i, X val) {
      data_[leaf_index(i)] = val;
   }

   // セグメント木の初期構築
   // @pre leafの値が設定されていること
   void build();

   // i番目の要素の完全二分木配列data_でのleaf位置を返す
   size_t leaf_index(size_t i) const {
      return i + leaf_size_ - 1;
   }

   // 添え字区間[a, b)の要素に対する二項演算結果を返す
   // @param k 現在のノード位置
   // @param [l, r) data_[k]が表している区間
   X sub_query(size_t a, size_t b, size_t k, size_t l, size_t r) const;

   vector<X> data_;  // 完全二分木を格納する配列
   function<X(X, X)> oper_;
   X unit_;            // 二項演算子の単位元
   size_t leaf_size_;  // 管理する配列のサイズ(=セグメント木のリーフ数)
};

template <class X>
SegmentTree<X>::SegmentTree(const vector<X>& val_list, std::function<X(X, X)> oper, const X unit)
    : oper_(oper), unit_(unit), leaf_size_(1) {
   // val_listの要素をleafとする完全二分木を作る
   while (val_list.size() > leaf_size_) {
      leaf_size_ *= 2;
   }

   size_t tree_size = 2 * leaf_size_;
   data_.resize(tree_size);
   fill(data_.begin(), data_.end(), unit);

   for (size_t i = 0; i < val_list.size(); i++) {
      set(i, val_list[i]);
   }

   build();
}

template <class X>
void SegmentTree<X>::build() {
   for (long long i = (long long)leaf_size_ - 2; i >= 0; i--) {
      data_[i] = oper_(data_[2 * i + 1], data_[2 * i + 2]);
   }
}

template <class X>
void SegmentTree<X>::update(size_t i, X val) {
   set(i, val);

   auto index = leaf_index(i);

   while (index > 0) {
      // 親ノードのindex
      index = (index - 1) / 2;
      data_[index] = oper_(data_[index * 2 + 1], data_[index * 2 + 2]);
   }
}

template <class X>
X SegmentTree<X>::sub_query(size_t a, size_t b, size_t k, size_t l, size_t r) const {
   // 演算を行う範囲[a, b)と今いるノードkが表してる範囲[r, l)を比較する
   if (r <= a || b <= l) {
      // 自身の範囲が演算を行う範囲[a, b)外
      // -> 演算しなくて良いので単位元を返して終了
      return unit_;
   } else if (a <= l && r <= b) {
      // 自身の範囲が演算を行う範囲[a, b)に含まれる
      // -> 自身の値を返す
      return data_[k];
   } else {
      // 自身の範囲が演算を行う範囲[a, b)に一部含まれる
      // -> より範囲の狭い子ノードの結果を使う
      X val_l = sub_query(a, b, k * 2 + 1, l, (l + r) / 2);
      X val_r = sub_query(a, b, k * 2 + 2, (l + r) / 2, r);

      return oper_(val_l, val_r);
   }
}

// SparseTable(min/max/gcd/lcmなど結合則と冪等性があれば適用可能)
// 前処理: 計算量、空間量 O(N log N)
// クエリ: 計算量O(1)
// @ref https://ikatakos.com/pot/programming_algorithm/data_structure/sparse_table
// @ref https://kopricky.github.io/code/DataStructure_Basic/sparse_table.html
template <class T>
class SparseTable {
  public:
   SparseTable(const vector<T>& seq, const function<T(T, T)>& fx);

   // 半開区間[l, r)(l, rは0-indexed)の演算結果を返す
   // @pre 0<=l<r, r<=N
   T query(const size_t l, const size_t r) const;

  private:
   // log_table_[i]: 2^kのうちiを越えない最大のもの(i=1,2,...,n)
   vector<size_t> log_table_;

   // table[l][i]: 添え字iを起点に長さ2^lの区間の最小値を記録
   vector<vector<T>> table_;

   // X * X -> Xとなる演算子(ex. min, max, gcd, lcd)
   function<T(T, T)> fx_;
};

template <class T>
SparseTable<T>::SparseTable(const vector<T>& seq, const function<T(T, T)>& fx)
    : fx_(fx) {
   assert(!seq.empty());

   const size_t N = seq.size();

   // 2^kのうちiを越えない最大のものを計算しておく
   log_table_.resize(N + 1);
   log_table_[0] = log_table_[1] = 0;

   for (size_t i = 2; i <= N; i++) {
      log_table_[i] = log_table_[i >> 1] + 1;
   }

   table_.resize(log_table_[N] + 1, vector<T>(N, 0));

   // 事前計算しておく区間(l=1, 2,.., 2^k<N)幅の最大
   for (size_t i = 0; i < N; i++) {
      table_[0][i] = seq[i];
   }

   const size_t max_log_index = log_table_[N];

   for (size_t l = 1; l <= max_log_index; l++) {
      const size_t L = 1ULL << l;
      const size_t L_prev = 1ULL << (l - 1);

      for (size_t i = 0; i < N; i++) {
         if (!(i + L <= N)) {
            break;
         }

         table_[l][i] = fx_(table_[l - 1][i], table_[l - 1][i + L_prev]);
      }
   }
}

template <class T>
T SparseTable<T>::query(const size_t l, const size_t r) const {
   assert(0 <= l && l < r);

   // 区間幅([l, r)に含まれる最大の2^k)
   const auto L_ind = log_table_[r - l];
   const auto L = 1ULL << L_ind;

   // 始点がlで区間幅Lと終点がr(=始点がr-L)で区間幅Lを重ねたものが求めたい区間になる
   const auto i1 = l;      // 始点 l, 区間幅 L
   const auto i2 = r - L;  // 終点 r(=始点 r-L), 区間幅 L

   auto value = fx_(table_[L_ind][i1], table_[L_ind][i2]);

   return value;
}

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
   void UpdateNodeWeight(const int node, const long long weight);

   // 辺の重みを更新する
   // 計算量: O(log N)
   void UpdateEdgeWeight(int edge_index, const long long weight);

   // Euler tour順序で部分木クエリ/パスクエリ/LCA/2点間クエリ用のデータを生成する
   // 計算量: O(N log N)
   // @note LCAが不要ならばSparseTableの生成をskipすることでO(N)に高速化可能
   void Build(int root_node);

   // 部分木のノード数を返す
   // @note 木なので辺数はノード数 - 1
   // 計算量: O(1)
   int GetSubTreeSize(const int node) const;

   // 部分木クエリ(ノードと辺の重みの合計)を返す
   // @param node ノード番号(1-index)
   // 計算量: O(log N)
   long long SubTreeQuery(const int node) const;

   // パスクエリ(rootからノードへの合計)を返す
   // 計算量: O(log N)
   long long PathQuery(const int node) const;

   // ノードの深さを返す
   // 計算量: O(1)
   int GetDepth(const int node) const;

   // 2ノード間の最近共通祖先(LCA)を求める
   // 計算量: O(1)
   int GetLCA(const int node_1, const int node_2) const;

   // 2ノード間のパスクエリを返す(node_1からnode_2への最短路上の合計を返す)
   // 計算量: O(log N)
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

// 辺の重みを取得する
long long EulerTour::GetEdgeWeight(const int edge_index) const {
   return get<2>(edge_list_[edge_index]);
}

int main() {
   int N, Q;
   cin >> N >> Q;

   vector<ll> al(N);
   cin >> al;

   vector<EulerTour::Edge> edge_list;

   for (int i = 1; i < N; i++) {
      int u, v;
      cin >> u >> v;
      u++;
      v++;
      edge_list.emplace_back(u, v, 0);
   }

   EulerTour euler_tour(edge_list);

   rep(i, N) {
      euler_tour.SetNodeWeight(i + 1, al[i]);
   }

   euler_tour.Build(1);

   rep(q, Q) {
      int t;
      cin >> t;

      if (t == 0) {
         ll u, x;
         cin >> u >> x;
         u++;

         auto cur_x = euler_tour.GetNodeWeight(u);
         euler_tour.UpdateNodeWeight(u, cur_x + x);
      } else {
         int u, v;
         cin >> u >> v;
         u++;
         v++;

         ll ans = euler_tour.PathQuery(u, v);
         cout << ans << endl;
      }
   }

   return 0;
}