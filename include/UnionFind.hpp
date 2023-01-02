#pragma once

#include <vector>
#include <map>

using namespace std;

// [Start] UnionFind
// [Prefix] union-find-class
// UnionFindで素集合を管理するクラス
// ノードIDは1-indexed
// [Skip] @ref https://www.slideshare.net/chokudai/union-find-49066733/1
// [Verified] n<=10^4, query<=10^5, DSL_1_A(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_1_A&lang=ja)
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
