#include <algorithm>
#include <limits>
#include <iostream>
#include <vector>
#include <cassert>
#include <map>
#include <queue>
#include <unordered_map>

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

// BFSで単一始点最短路を求める
// 計算量: O(N+E)
// 非連結成分には numeric_limits<long long>::max() が設定される
vector<long long> ShortestPathBFS(const vector<vector<int>>& adj_list, const int start) {
   // 重みリストの初期化
   constexpr long long INF = numeric_limits<long long>::max();
   int L = (int)adj_list.size();
   vector<long long> min_weight_list(L, INF);

   min_weight_list[start] = 0;

   // 最短路が求まったノードを管理する
   queue<int> node_queue;
   node_queue.push(start);

   while (!node_queue.empty()) {
      const auto min_node = node_queue.front();
      node_queue.pop();

      // 隣接するノードうち未訪問のものを更新する
      for (const auto node_to : adj_list[min_node]) {
         if (min_weight_list[node_to] == INF) {
            min_weight_list[node_to] = min_weight_list[min_node] + 1;
            node_queue.push(node_to);
         }
      }
   }

   return min_weight_list;
}

// ノード間の最短経路を求める(重み付き用)
// @param start, node: 最短経路を求めるノード
// @param min_weight_list: startから各ノードの最短距離が格納されたテーブル
// @retval node_path: startノードからendノードまでの最短経路順に並べたノードリスト
// @note 計算量: O(E)
// Unverified
vector<int> FindShortestPath(const int start, const int end, const vector<vector<pair<int, long long>>>& adj_list, const vector<long long>& min_weight_list) {
   int N = (int)adj_list.size() - 1;

   // to -> fromの逆順隣接リストを作る
   using Edge = pair<int, long long>;
   vector<vector<Edge>> rev_adj_list(N + 1, vector<Edge>());

   for (int from = 1; from <= N; from++) {
      for (auto [to, weight] : adj_list[from]) {
         rev_adj_list[to].emplace_back(from, weight);
      }
   }

   vector<int> back_path;
   int node = end;

   back_path.push_back(node);

   while (min_weight_list[node] != 0) {
      for (auto [from, weight] : rev_adj_list[node]) {
         if (min_weight_list[node] == min_weight_list[from] + weight) {
            back_path.push_back(from);
            node = from;
            break;
         }
      }
   }

   reverse(back_path.begin(), back_path.end());
   return back_path;
}

// ノード間の最短経路を求める(重みなし用)
// @param start, node: 最短経路を求めるノード
// @param min_weight_list: startから各ノードの最短距離が格納されたテーブル
// @retval node_path: startノードからendノードまでの最短経路順に並べたノードリスト
// @note 計算量: O(E)
vector<int> FindShortestPath(const int start, const int end, const vector<vector<int>>& adj_list, const vector<long long>& min_weight_list) {
   int N = (int)adj_list.size() - 1;

   using Edge = pair<int, long long>;
   vector<vector<Edge>> weight_adj_list(N + 1, vector<Edge>());

   for (int node = 1; node <= N; node++) {
      for (auto to : adj_list[node]) {
         weight_adj_list[node].emplace_back(to, 1);
      }
   }

   return FindShortestPath(start, end, weight_adj_list, min_weight_list);
}

// 無向グラフで閉路を求める
// @retval 閉路のリスト。閉路は(start_node, edge index list)のpairで表現
// @note 連結成分ごとに閉路があればそのうち1つを返す
// @note 自己ループ、多重辺があっても可能
// 計算量: ほぼO(N + E)
// 依存ライブラリ: UnionFind, Shortest path(BFS), Find shortest path
vector<pair<int, vector<int>>> FindLoopUndirected(int N, const vector<pair<int, int>>& edge_list) {
   UnionFind uf(N);

   vector<bool> root_loop(N + 1, false);
   vector<vector<int>> adj_list(N + 1);
   int M = edge_list.size();
   vector<unordered_map<int, int>> node_edge_map(N + 1);  // node_edge_map[i][j] -> edge(i, j)のindex
   vector<pair<int, vector<int>>> loop_group;

   auto Unite = [&](int edge_index) {
      auto [u, v] = edge_list[edge_index];

      uf.Unite(u, v);

      adj_list[u].emplace_back(v);
      adj_list[v].emplace_back(u);

      node_edge_map[u][v] = edge_index;
      node_edge_map[v][u] = edge_index;
   };

   for (int i = 0; i < M; i++) {
      auto [u, v] = edge_list[i];

      auto root_u = uf.root(u);
      auto root_v = uf.root(v);

      if (root_loop[root_u] || root_loop[root_v]) {
         // すでにloopを抽出済
         Unite(i);
         root_loop[root_u] = root_loop[root_v] = true;
         continue;
      }

      if (uf.IsSameGroup(u, v)) {
         // u -> vのpathを求める
         auto min_dist_tbl = ShortestPathBFS(adj_list, u);
         auto loop_node_list = FindShortestPath(u, v, adj_list, min_dist_tbl);
         vector<int> loop_edge_index_list;

         for (int l = 1; l < (int)loop_node_list.size(); l++) {
            auto from = loop_node_list[l - 1];
            auto to = loop_node_list[l];

            auto e = node_edge_map[from][to];
            loop_edge_index_list.emplace_back(e);
         }

         // u-vをつないで閉路を作る
         loop_edge_index_list.emplace_back(i);
         root_loop[root_u] = root_loop[root_v] = true;

         loop_group.emplace_back(u, loop_edge_index_list);
      }

      Unite(i);
   }

   return loop_group;
}

int main() {
   int N, M;
   cin >> N >> M;

   // N: nodes, M: edges
   using Edge = pair<int, int>;
   vector<Edge> edge_list;

   for (int i = 0; i < M; i++) {
      int u, v;
      cin >> u >> v;

      u++;
      v++;

      edge_list.emplace_back(u, v);
   }

   auto loop_group = FindLoopUndirected(N, edge_list);

   if (loop_group.empty()) {
      cout << -1 << endl;
   } else {
      auto [node, edge_index_list] = loop_group[0];
      cout << edge_index_list.size() << endl;

      vector<int> node_list;

      node_list.emplace_back(node - 1);

      for (auto e : edge_index_list) {
         auto [u, v] = edge_list[e];

         if (u == node) {
            node_list.emplace_back(v - 1);
            node = v;
         } else {
            node_list.emplace_back(u - 1);
            node = u;
         }
      }

      node_list.pop_back();

      cout << node_list << endl;
      cout << edge_index_list << endl;
   }

   return 0;
}