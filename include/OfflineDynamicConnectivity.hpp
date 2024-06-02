#pragma once

#include <cassert>
#include <vector>
using namespace std;

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

// [Start] OfflineDynamicConnectivity
// [Prefix] offline-dynamic-connectivity-class
// [Skip] @ref https://ei1333.github.io/luzhiled/snippets/other/offline-dynamic-connectivity.html
// [Skip] @ref https://onedrive.live.com/view?id=CD510BE428DBA1E7!106&resid=CD510BE428DBA1E7!106&authkey=!AFD6EO1-AReoPBk&wdSlideId=256&wdModeSwitchTime=1717304008277&wdo=2&cid=cd510be428dba1e7
// [Verified] N <= 2 * 10^5, Q <= 2 * 10^5: ABC 356 F - Distance Component Size Query(https://atcoder.jp/contests/abc356/tasks/abc356_f)
// 依存ライブラリ: Undo可能UnionFind(union-find-undo-class)
class OfflineDynamicConnectivity {
  public:
   using Edge = pair<int, int>;

   OfflineDynamicConnectivity(int N, int Q);

   // 時刻tにノードuとノードvを結ぶ辺を追加する
   // ノードは1-indexed
   void Insert(int t, int u, int v);

   // 時刻tにノードuとノードvを結ぶ辺を削除する
   // ノードは1-indexed
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
      assert(false);
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
// [End] OfflineDynamicConnectivity
