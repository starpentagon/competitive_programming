#pragma once

#include <cassert>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

// [Start] UnionFind
// [Prefix] union-find-class
// UnionFindで素集合を管理するクラス
// ノードIDは1-indexed
// [Skip] @ref https://www.slideshare.net/chokudai/union-find-49066733/1
// [Verified] n<=10^4, query<=10^5, DSL_1_A(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_1_A&lang=ja)
// [Verified] N,Q<=2*10^5, Unionfind(https://judge.yosupo.jp/problem/unionfind)
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
// [End] UnionFind

// [Start] UnionFindUndo
// [Prefix] union-find-undo-class
// Undo可能UnionFindで素集合を管理するクラス(union by sizeのみ行うことでUnion時の変更箇所が2個所になるようにして履歴を持つ)
// ノードIDは1-indexed
// [Skip] @ref https://ei1333.github.io/luzhiled/snippets/structure/union-find.html
// [Verified] n<=2*10^5, Q<=2*10^5, Persistent Unionfind(https://judge.yosupo.jp/problem/persistent_unionfind)
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

// [End] UnionFind
