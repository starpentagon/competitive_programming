#pragma GCC target("arch=skylake-avx512")
#pragma GCC optimize("O3")

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

template <class T>
class PosCompression {
  public:
   // dataを圧縮する
   // 計算量: O(N log N)
   PosCompression(const vector<T>& data);

   PosCompression() {
   }

   // 圧縮前の値valを圧縮した値を返す
   // @pre valが登録されていること
   size_t Encode(const T& val) const;

   // 圧縮後の値indexの圧縮前の値を返す
   T Decode(const size_t index) const;

   // 要素数を返す
   size_t size() const {
      return comp_.size();
   }

  private:
   vector<T> comp_;  // comp_[index]: dataのユニークな値を昇順ソートした時にindex番目の値
};

template <class T>
PosCompression<T>::PosCompression(const vector<T>& data)
    : comp_(data) {
   // 昇順ソートし重複を削除する
   sort(comp_.begin(), comp_.end());
   comp_.erase(unique(comp_.begin(), comp_.end()), comp_.end());
}

template <class T>
size_t PosCompression<T>::Encode(const T& val) const {
   size_t index = lower_bound(comp_.begin(), comp_.end(), val) - comp_.begin();
   return index;
}

template <class T>
T PosCompression<T>::Decode(const size_t index) const {
   assert(0 <= index && index < comp_.size());
   return comp_[index];
}

// Undo可能UnionFindで素集合を管理するクラス(union by sizeのみ行うことでUnion時の変更箇所が2個所になるようにして履歴を持つ)
// ノードIDは1-indexed
class UnionFindUndo {
  public:
   UnionFindUndo(const size_t N);

   // node_1とnode_2が所属する集合を結合する(Union by size)
   // 計算量: O(log N)
   void Unite(size_t node_1, size_t node_2);

   // node_1とnode_2が同じ集合に入っているか
   // 計算量: O(log N)
   bool IsSameGroup(size_t node_1, size_t node_2) const;

   // nodeが所属する集合のサイズを返す
   // 計算量: O(log N)
   size_t size(size_t node) const;

   // 各グループを返す(親ノードの番号 -> 子のリスト)
   // 計算量: O(N log N)
   map<size_t, vector<size_t>> EnumGroup() const;

   // nodeの親のnode_idを返す
   // 計算量: O(log N)
   size_t root(size_t node) const;

   // Uniteを取り消す
   // 計算量: O(1)
   void Undo();

   // 現時点の状態をスナップショットで残す
   // note: これ以前の履歴は戻せなくなる
   void Snapshot() {
      history_.clear();
   }

   // スナップショットを取った状態に戻す
   void Rollback() {
      while (!history_.empty()) {
         Undo();
      }
   }

  private:
   // ノードの数
   size_t N_;

   // data_[i]: iの親ノードのノードID, rootの場合は所属するノード数の負数
   vector<int> data_;

   // Undo用にUnion前の履歴(変更する親ノードindex, 変更前の値)を持っておく
   vector<pair<int, int>> history_;
};

UnionFindUndo::UnionFindUndo(const size_t N)
    : N_(N), data_(N + 1, -1) {
}

size_t UnionFindUndo::root(size_t node) const {
   assert(1 <= node && node <= N_);

   if (data_[node] < 0) return node;
   return root(data_[node]);
}

bool UnionFindUndo::IsSameGroup(size_t node_1, size_t node_2) const {
   auto parent_1 = root(node_1);
   auto parent_2 = root(node_2);

   return parent_1 == parent_2;
}

void UnionFindUndo::Unite(size_t node_1, size_t node_2) {
   auto root_1 = root(node_1);
   auto root_2 = root(node_2);

   history_.emplace_back(root_1, data_[root_1]);
   history_.emplace_back(root_2, data_[root_2]);

   if (root_1 == root_2) return;

   // サイズの小さい方を大きいにつなぐ
   // マージテクで高さがO(log N)に押さえられる
   auto size_1 = abs(data_[root_1]);
   auto size_2 = abs(data_[root_2]);

   if (size_1 < size_2) swap(root_1, root_2);

   data_[root_1] += data_[root_2];
   data_[root_2] = root_1;
}

size_t UnionFindUndo::size(size_t node) const {
   auto p = root(node);
   return -data_[p];
}

map<size_t, vector<size_t>> UnionFindUndo::EnumGroup() const {
   map<size_t, vector<size_t>> group;

   for (size_t i = 1; i <= N_; i++) {
      auto parent = root(i);
      group[parent].push_back(i);
   }

   return group;
}

void UnionFindUndo::Undo() {
   assert(history_.size() >= 2);

   auto [root_2, data_2] = history_.back();
   history_.pop_back();
   data_[root_2] = data_2;

   auto [root_1, data_1] = history_.back();
   history_.pop_back();
   data_[root_1] = data_1;
}

// 依存ライブラリ: Undo可能UnionFind(union-find-undo-class)
class OfflineDynamicConnectivity {
  public:
   using Edge = pair<int, int>;

   OfflineDynamicConnectivity(int N, int Q);

   // 時刻tにノードuとノードvを結ぶ辺を追加する
   void Insert(int t, int u, int v);

   // 時刻tにノードuとノードvを結ぶ辺を削除する
   void Erase(int t, int u, int v);

   // クエリをすべて与えたうえで呼び出す
   void Build();

   // 各クエリでの回答を記録する
   // query_input: クエリの入力(ex. クエリ番号 -> ノード番号)
   // query_ans: クエリの回答(ex. クエリ番号 -> ノードの連結成分のサイズ)
   void Run(const map<int, ll>& query_input, map<int, ll>& query_ans, int node = 0);

  protected:
   // セグメント木に時間ごとに存在する辺の情報を追加する
   void Add(int a, int b, const Edge& e, int k, int l, int r);
   void Add(int a, int b, const Edge& e) {
      Add(a, b, e, 0, 0, S_);
   }

   int N_;  // ノード数
   int Q_;  // クエリ数
   int S_;  // セグメント木のサイズ

   UnionFindUndo uf_;
   vector<vector<Edge>> seg_;  // セグメント木

   map<Edge, int> edge_cnt_;
   map<Edge, int> edge_appear_;

   using TimeInterval = pair<int, int>;
   vector<pair<TimeInterval, Edge>> edge_time_info_;  // 辺が存在する時間情報
};

OfflineDynamicConnectivity::OfflineDynamicConnectivity(int N, int Q)
    : N_(N), Q_(Q), S_(1), uf_(N) {
   while (S_ < Q_) S_ <<= 1;
   seg_.resize(S_ * 2 - 1);
}

void OfflineDynamicConnectivity::Insert(int t, int u, int v) {
   if (u > v) swap(u, v);
   Edge e{u, v};

   if (edge_cnt_[e] == 0) edge_appear_[e] = t;
   edge_cnt_[e]++;
}

void OfflineDynamicConnectivity::Erase(int t, int u, int v) {
   if (u > v) swap(u, v);
   Edge e{u, v};

   edge_cnt_[e]--;
   if (edge_cnt_[e] == 0) {
      edge_time_info_.emplace_back(make_pair(edge_appear_[e], t), e);
   }
}

void OfflineDynamicConnectivity::Add(int a, int b, const Edge& e, int k, int l, int r) {
   if (r <= a || b <= l) return;

   if (a <= l && r <= b) {
      seg_[k].push_back(e);
      return;
   }

   Add(a, b, e, k * 2 + 1, l, (l + r) / 2);
   Add(a, b, e, k * 2 + 2, (l + r) / 2, r);
}

void OfflineDynamicConnectivity::Build() {
   for (auto [e, cnt] : edge_cnt_) {
      if (cnt > 0) {
         edge_time_info_.emplace_back(make_pair(edge_appear_[e], Q_), e);
      }
   }

   for (const auto& [time_interval, e] : edge_time_info_) {
      auto [l, r] = time_interval;
      Add(l, r, e);
   }
}

void OfflineDynamicConnectivity::Run(const map<int, ll>& query_input, map<int, ll>& query_ans, int node) {
   for (const auto& [u, v] : seg_[node]) {
      uf_.Unite(u, v);
   }

   if (node < S_ - 1) {
      Run(query_input, query_ans, node * 2 + 1);
      Run(query_input, query_ans, node * 2 + 2);
   } else if (node - (S_ - 1) < Q_) {
      // セグメント木のLeaf = q回目のクエリを実行したときの状態になっている
      int query_index = node - (S_ - 1);

      // ここにクエリに対する処理を記述する
      if (query_input.count(query_index)) {
         int node = query_input.at(query_index);
         int ans = uf_.size(node);
         query_ans[query_index] = ans;
      }
   }

   for (int i = 0; i < (int)seg_[node].size(); i++) {
      uf_.Undo();
   }
}

// valより大きい最小の要素を返す
// @note 条件を満たす要素がない場合はendイテレータを返す
template <class T>
typename set<T>::const_iterator find_min_greater(const set<T>& val_set, const T& val) {
   auto it = val_set.upper_bound(val);
   return it;
}

// valより小さい最大の要素を返す
// @note 条件を満たす要素がない場合はendイテレータを返す
template <class T>
typename set<T>::const_iterator find_max_less(const set<T>& val_set, const T& val) {
   // val以上の要素のひとつ前の要素 = valより小さい最大の要素
   auto it = val_set.lower_bound(val);

   if (it == val_set.begin()) {
      return val_set.end();
   }

   --it;

   return it;
}

int main() {
   ll Q, K;
   cin >> Q >> K;

   using Query = pair<ll, ll>;
   vector<Query> query_list;

   vector<ll> x_list;

   rep(q, Q) {
      ll t, x;
      cin >> t >> x;
      query_list.emplace_back(t, x);
      x_list.push_back(x);
   }

   x_list.emplace_back(-1LL - K);
   x_list.emplace_back(1e18 + K + 10);

   PosCompression<ll> comp(x_list);
   ll C = comp.size();

   OfflineDynamicConnectivity odc(C, Q);
   set<ll> x_set;

   // 番兵
   x_set.insert(-1 - K);
   x_set.insert((ll)1e18 + K + 10);

   auto add = [&](int q, ll x) {
      x_set.insert(x);

      ll x_prev = *find_max_less(x_set, x);
      ll x_next = *find_min_greater(x_set, x);

      ll index = comp.Encode(x) + 1;
      ll index_prev = comp.Encode(x_prev) + 1;
      ll index_next = comp.Encode(x_next) + 1;

      if (x_next - x_prev <= K) {
         odc.Erase(q, index_prev, index_next);
      }

      if (x - x_prev <= K) {
         odc.Insert(q, index, index_prev);
      }

      if (x_next - x <= K) {
         odc.Insert(q, index, index_next);
      }
   };

   auto del = [&](int q, ll x) {
      x_set.erase(x);

      ll x_prev = *find_max_less(x_set, x);
      ll x_next = *find_min_greater(x_set, x);

      ll index = comp.Encode(x) + 1;
      ll index_prev = comp.Encode(x_prev) + 1;
      ll index_next = comp.Encode(x_next) + 1;

      if (x - x_prev <= K) {
         odc.Erase(q, index, index_prev);
      }

      if (x_next - x <= K) {
         odc.Erase(q, index, index_next);
      }

      if (x_next - x_prev <= K) {
         odc.Insert(q, index_prev, index_next);
      }
   };

   map<int, ll> query_input;

   rep(q, Q) {
      auto [t, x] = query_list[q];

      if (t == 1) {
         if (!x_set.count(x)) {
            add(q, x);
         } else {
            del(q, x);
         }
      } else {
         query_input[q] = comp.Encode(x) + 1;
      }
   }

   odc.Build();

   map<int, ll> query_ans;
   odc.Run(query_input, query_ans);

   rep(q, Q) {
      auto [t, x] = query_list[q];
      if (t == 2) {
         cout << query_ans[q] << endl;
      }
   }

   return 0;
}
