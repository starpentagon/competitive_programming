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

int main() {
   using Edge = tuple<int, int, int>;
   unordered_map<int, vector<Edge>> graph_edges;  // graph_edges[i]: G_iに辺(a, b)を追加したグラフをG_jとする

   using Query = tuple<int, int, int>;  // G_iでu, vが連結かどうか
   map<Query, int> answer;

   int N, Q;
   cin >> N >> Q;

   map<int, vector<pair<int, int>>> graph_queries;
   vector<Query> query_list;

   rep(q, Q) {
      int t, k, u, v;

      cin >> t >> k >> u >> v;
      u++;
      v++;

      if (t == 0) {
         graph_edges[k].emplace_back(u, v, q);
      } else {
         graph_queries[k].emplace_back(u, v);
         query_list.emplace_back(k, u, v);
      }
   }

   UnionFindUndo uf(N);

   auto solve = [&](auto solve, int g) -> void {
      if (graph_queries.count(g)) {
         for (auto [u, v] : graph_queries[g]) {
            answer[{g, u, v}] = uf.IsSameGroup(u, v);
         }
      }

      for (auto [u, v, q] : graph_edges[g]) {
         uf.Unite(u, v);
         solve(solve, q);
         uf.Undo();
      }
   };

   solve(solve, -1);

   for (auto [g, u, v] : query_list) {
      cout << answer[{g, u, v}] << endl;
   }

   return 0;
}
