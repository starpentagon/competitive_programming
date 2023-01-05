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

// 指定回数移動後にどのノードにいるかを返す
// @note 事前計算: O(N), startからMoveCount回の移動計算: O(1)
// @pre ノードstartが所属する連結成分はFunctional Graphであること
// @retval [out_loop, in_loop] : startを起点にloopに入るまでの移動履歴、loopに入った後の移動履歴
pair<vector<int>, vector<int>> MoveOnLoopPrep(const int start, const vector<vector<int>>& adj_list, const vector<pair<int, int>>& edge_list, const vector<int>& loop_edge_index_list) {
   int N = adj_list.size() - 1;
   vector<bool> loop_node(N + 1, false);

   for (auto e : loop_edge_index_list) {
      auto [from, to] = edge_list[e];
      loop_node[from] = true;
   }

   int node = start;
   auto loop_size = loop_edge_index_list.size();

   vector<int> out_loop_node, in_loop_node;

   while (true) {
      if (loop_node[node]) {
         if (in_loop_node.size() == loop_size) {
            break;
         }

         in_loop_node.emplace_back(node);
      } else {
         out_loop_node.emplace_back(node);
      }

      assert(adj_list[node].size() == 1);
      node = adj_list[node][0];
   }

   return {out_loop_node, in_loop_node};
}

// 指定回数を移動した後のノード番号を返す
int MoveOnLoop(const vector<int>& out_loop_node, const vector<int>& in_loop_node, long long move_count) {
   if (move_count < (long long)out_loop_node.size()) {
      return out_loop_node[move_count];
   }

   move_count -= out_loop_node.size();
   move_count %= in_loop_node.size();

   return in_loop_node[move_count];
}

// val以上で最小の要素を返す
// @note 条件を満たす要素がない場合はendイテレータを返す
// @note val_listはソート済であること
template <class T>
typename vector<T>::const_iterator find_min_greater_eq(const vector<T>& val_list, const T& val) {
   auto it = lower_bound(val_list.begin(), val_list.end(), val);
   return it;
}

// ToDo: 「E - Takahashi's Anguish」(https://atcoder.jp/contests/abc256/tasks/abc256_e)でのverify
// 有向グラフで閉路を求める
// @retval 閉路のリスト。閉路はmap[node]: to_nodeのマップで表現
// @note 連結成分ごとに閉路があればそのうち1つを返す
// @note 自己ループがあっても可能
// @retval 閉路のリスト。閉路はedge index listのlistで表現
vector<vector<int>> FindLoopDirected(int N, const vector<pair<int, int>>& edge_list) {
   using EdgeInfo = pair<int, int>;  // to, edge index
   vector<vector<EdgeInfo>> adj_list(N + 1);
   int M = edge_list.size();

   for (int i = 0; i < M; i++) {
      auto [u, v] = edge_list[i];

      adj_list[u].emplace_back(v, i);
   }

   vector<bool> visited(N + 1, false);   // 1-indexed
   vector<bool> finished(N + 1, false);  // 1-indexed

   // @retval -1: loopはない, 0: loop復元済: 1以上: ループ復元中でループの開始ノード番号
   auto dfs = [&](auto dfs, int node, vector<int>& loop_edge_index_list) -> int {
      int ret = -1;
      visited[node] = true;

      for (auto [n_node, edge_index] : adj_list[node]) {
         if (finished[n_node]) continue;

         if (visited[n_node] && !finished[n_node]) {
            // n_nodeを開始ノードとするloopが存在する
            loop_edge_index_list.emplace_back(edge_index);
            ret = n_node;

            if (n_node == node) ret = 0;
            break;
         }

         ret = dfs(dfs, n_node, loop_edge_index_list);

         if (ret != -1) {
            if (ret >= 1) {
               loop_edge_index_list.emplace_back(edge_index);
            }

            if (ret == node) {
               ret = 0;
            }
            break;
         }
      }

      finished[node] = true;
      return ret;
   };

   vector<vector<int>> loop_group;

   for (int node = 1; node <= N; node++) {
      vector<int> loop_edge_index_list;
      auto loop_node = dfs(dfs, node, loop_edge_index_list);

      if (loop_node != -1) {
         reverse(loop_edge_index_list.begin(), loop_edge_index_list.end());
         loop_group.emplace_back(loop_edge_index_list);
      }
   }

   return loop_group;
}

int main() {
   ll N, Q, X;
   cin >> N >> Q >> X;

   vector<ll> wl(2 * N), cum_sum(2 * N + 1, 0);

   rep(i, N) {
      ll w;
      cin >> w;
      wl[i] = wl[i + N] = w;
   }

   for (int i = 0; i < 2 * N; i++) {
      cum_sum[i + 1] = cum_sum[i] + wl[i];
   }

   auto next_index = [&](const int now) -> pair<int, int> {
      ll cycle = X / cum_sum[N];
      ll now_cum_weight = cum_sum[now];
      ll target_weight = now_cum_weight + (X % cum_sum[N]);

      auto it = find_min_greater_eq(cum_sum, target_weight);
      auto ni = it - cum_sum.begin();

      int potatos = cycle * N + (ni - now);
      ni %= N;

      return {ni, potatos};
   };

   vector<vector<int>> adj_list(N + 1);
   vector<pair<int, int>> edge_list;

   auto add_edge = [&](int from, int to) {
      from++;
      to++;

      adj_list[from].emplace_back(to);
      edge_list.emplace_back(from, to);
   };

   vector<bool> visited(N, false);
   vector<int> node_potatos(N);
   int index = 0;

   while (!visited[index]) {
      visited[index] = true;

      auto [ni, p] = next_index(index);
      node_potatos[index] = p;

      add_edge(index, ni);

      index = ni;
   }

   auto loop_edge_list_group = FindLoopDirected(N, edge_list);
   assert(loop_edge_list_group.size() == 1);

   auto loop_edge_list = loop_edge_list_group[0];
   auto [out_loop_node, in_loop_node] = MoveOnLoopPrep(1, adj_list, edge_list, loop_edge_list);

   rep(q, Q) {
      ll K;
      cin >> K;

      auto node = MoveOnLoop(out_loop_node, in_loop_node, K - 1);
      auto ans = node_potatos[node - 1];

      cout << ans << endl;
   }

   return 0;
}