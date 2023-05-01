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

class StronglyConnectedComponents {
  public:
   using WeightedEdge = pair<int, long long>;

   // @param N 頂点数
   // @param adj_list 隣接リスト(重みあり)
   // 計算量: O(N+E)
   StronglyConnectedComponents(const vector<vector<WeightedEdge>>& adj_list);

   // @param N 頂点数
   // @param adj_list 隣接リスト(重みなし)
   // 計算量: O(N+E)
   StronglyConnectedComponents(const vector<vector<int>>& adj_list);

   // 強連結成分数を返す
   // 計算量: O(1)
   int GetSCCSize() const;

   // 強連結成分のグラフを返す
   // @note DAGになっている
   // 計算量: O(1)
   vector<vector<int>> GetSCCGraph() const;

   // 頂点nodeが所属する強連結成分No(1-indexed)を返す
   // 計算量: O(1)
   int GetNodeSCCNo(int node) const;

   // 「SCC番号 -> SCCに所属するノード番号リスト」の一覧を返す
   // 計算量: O(1)
   vector<vector<int>> GetSCCNoedGroup() const;

  protected:
   // 強連結成分を求める
   void Build();

   int N_;  // 頂点数

   vector<vector<WeightedEdge>> org_adj_list_;  // グラフの隣接リスト
   vector<vector<WeightedEdge>> rev_adj_list_;  // 逆辺グラフの隣接リスト

   vector<int> node_to_scc_no_;          // nodeが所属する強連結成分番号
   vector<vector<int>> scc_adj_list_;    // 強連結成分の隣接リスト(DAGになっている)
   vector<vector<int>> scc_node_group_;  // 強連結成分 -> 所属するノード番号リスト
};

StronglyConnectedComponents::StronglyConnectedComponents(const vector<vector<WeightedEdge>>& adj_list)
    : N_(adj_list.size() - 1), org_adj_list_(adj_list), rev_adj_list_(N_ + 1) {
   assert(!adj_list.empty());

   for (int from = 1; from <= N_; from++) {
      for (const auto& [to, weight] : adj_list[from]) {
         rev_adj_list_[to].emplace_back(from, weight);
      }
   }

   Build();
}

StronglyConnectedComponents::StronglyConnectedComponents(const vector<vector<int>>& adj_list)
    : N_(adj_list.size() - 1), org_adj_list_(N_ + 1), rev_adj_list_(N_ + 1) {
   assert(!adj_list.empty());

   for (int from = 1; from <= N_; from++) {
      for (const auto& to : adj_list[from]) {
         org_adj_list_[from].emplace_back(to, 0);
         rev_adj_list_[to].emplace_back(from, 0);
      }
   }

   Build();
}

void StronglyConnectedComponents::Build() {
   // 帰りがけ順の番号を記録する
   vector<int> node_post_order(N_ + 1, -1);     // node -> 帰りがけ順の番号
   vector<int> post_order_to_node(N_ + 1, -1);  // 帰りがけ順 -> node

   int post_order = 0;
   vector<bool> post_order_visited(N_ + 1, false);

   auto post_ordering = [&](auto post_ordering, int node) -> void {
      assert(!post_order_visited[node]);
      post_order_visited[node] = true;

      for (const auto& [n_node, weight] : org_adj_list_[node]) {
         if (post_order_visited[n_node]) continue;
         post_ordering(post_ordering, n_node);
      }

      post_order++;
      node_post_order[node] = post_order;
      post_order_to_node[post_order] = node;
   };

   for (int node = 1; node <= N_; node++) {
      if (post_order_visited[node]) continue;
      post_ordering(post_ordering, node);
   }

   // 強連結成分を列挙する
   node_to_scc_no_.resize(N_ + 1, -1);
   int scc_no = 0;

   scc_adj_list_.resize(N_ + 1);

   auto scc_numbering = [&](auto scc_numbering, int node) -> void {
      assert(node_to_scc_no_[node] == -1);
      node_to_scc_no_[node] = scc_no;

      for (const auto& [n_node, weight] : rev_adj_list_[node]) {
         if (node_to_scc_no_[n_node] != -1) {
            if (node_to_scc_no_[n_node] != scc_no) {
               // 逆辺グラフで先に作られた成分へ移動できる
               // -> 元のグラフで先に作られたSCCから今のSCCへ移動可能
               // ただし、多重辺になる可能性があるため後で取り除く
               int from_scc = node_to_scc_no_[n_node];
               scc_adj_list_[from_scc].emplace_back(scc_no);
            }

            continue;
         }

         scc_numbering(scc_numbering, n_node);
      }
   };

   for (int post_order = N_; post_order >= 1; post_order--) {
      int node = post_order_to_node[post_order];

      if (node_to_scc_no_[node] != -1) continue;

      scc_no++;
      scc_numbering(scc_numbering, node);
   }

   scc_adj_list_.resize(scc_no + 1);

   // 多重辺になる可能性があるため除去する
   for (int i = 1; i <= scc_no; i++) {
      // (強連結成分は昇順で記録されているためソート不要)
      scc_adj_list_[i].erase(unique(scc_adj_list_[i].begin(), scc_adj_list_[i].end()), scc_adj_list_[i].end());
   }

   scc_node_group_.resize(scc_no + 1);

   for (int node = 1; node <= N_; node++) {
      int node_scc_no = GetNodeSCCNo(node);

      scc_node_group_[node_scc_no].emplace_back(node);
   }
}

int StronglyConnectedComponents::GetSCCSize() const {
   assert(!scc_adj_list_.empty());
   int scc_count = scc_adj_list_.size() - 1;
   return scc_count;
}

int StronglyConnectedComponents::GetNodeSCCNo(int node) const {
   assert(1 <= node && node <= N_);
   return node_to_scc_no_[node];
}

vector<vector<int>> StronglyConnectedComponents::GetSCCGraph() const {
   return scc_adj_list_;
}

vector<vector<int>> StronglyConnectedComponents::GetSCCNoedGroup() const {
   return scc_node_group_;
}

// 隣接リストからトポロジカルソートを行う
// @param adj_list 隣接リスト
// @retval sorted_list : トポロジカルソート後のノード番号リスト
// @retval true トポロジカルソート完了, false DAGでなくトポロジカルソート不可能
// @note 計算量: O(E + N)
pair<bool, vector<int>> TopologicalSort(const vector<vector<int>>& adj_list) {
   const int N = (int)adj_list.size() - 1;

   using Edge = pair<int, int>;  // from, to
   vector<Edge> edge_list;

   for (int node = 1; node <= N; node++) {
      for (auto to : adj_list[node]) {
         edge_list.emplace_back(node, to);
      }
   }

   vector<int> sorted_list;
   sorted_list.reserve(N);

   // ノードの入力次数
   vector<int> in_degree(N + 1, 0);

   // 隣接リスト(エッジインデックスのリスト)を作成
   vector<vector<int>> edge_index_list(N + 1, vector<int>());
   const int E = (int)edge_list.size();

   for (int i = 0; i < E; i++) {
      const auto [from, to] = edge_list[i];

      in_degree[to]++;
      edge_index_list[from].push_back(i);
   }

   // エッジが残っているかのフラグ
   vector<bool> edge_exist(E, true);

   // 入力次数が0のノードが始点になる
   queue<int> node_queue;

   for (int node = 1; node <= N; node++) {
      if (in_degree[node] == 0) {
         node_queue.push(node);
      }
   }

   while (!node_queue.empty()) {
      int node = node_queue.front();
      node_queue.pop();

      sorted_list.push_back(node);

      for (int edge_index : edge_index_list[node]) {
         if (!edge_exist[edge_index]) {
            continue;
         }

         edge_exist[edge_index] = false;

         const auto [from, to] = edge_list[edge_index];
         in_degree[to]--;

         if (in_degree[to] == 0) {
            node_queue.push(to);
         }
      }
   }

   bool all_edge_deleted = true;

   for (int i = 0; i < E; i++) {
      if (edge_exist[i]) {
         all_edge_deleted = false;
         break;
      }
   }

   return {all_edge_deleted, sorted_list};
}

// 重み付き版
pair<bool, vector<int>> TopologicalSort(const vector<vector<pair<int, long long>>>& adj_weight_list) {
   int N = adj_weight_list.size() - 1;
   vector<vector<int>> adj_list(N + 1);

   for (int u = 1; u <= N; u++) {
      for (auto [v, w] : adj_weight_list[u]) {
         adj_list[u].emplace_back(v);
      }
   }

   return TopologicalSort(adj_list);
}

int main() {
   int N, M;
   cin >> N >> M;

   // N: nodes, M: edges
   vector<vector<int>> adj_list(N + 1, vector<int>());

   for (int i = 0; i < M; i++) {
      int u, v;
      cin >> u >> v;

      u++;
      v++;

      adj_list[u].emplace_back(v);
   }

   StronglyConnectedComponents scc(adj_list);

   auto scc_node_group = scc.GetSCCNoedGroup();
   auto scc_graph = scc.GetSCCGraph();

   auto [flag, sorted_scc_no_list] = TopologicalSort(scc_graph);
   assert(flag);

   cout << scc.GetSCCSize() << endl;

   for (auto scc_no : sorted_scc_no_list) {
      int L = scc_node_group[scc_no].size();
      cout << L << " ";

      rep(i, L) {
         cout << scc_node_group[scc_no][i] - 1;
         if (i == L - 1)
            cout << endl;
         else
            cout << " ";
      }
   }

   return 0;
}