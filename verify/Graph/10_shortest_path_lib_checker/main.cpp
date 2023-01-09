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

// ダイクストラ法で単一始点最短路を求める
// @pre 各エッジの重みが非負であること
// 計算量: O(E + N log N)
// 非連結成分には numeric_limits<long long>::max() が設定される
vector<long long> ShortestPathDijkstra(const vector<vector<pair<int, long long>>>& adj_list, const int start) {
   // 重みリストの初期化
   int L = (int)adj_list.size();
   constexpr long long INF = numeric_limits<long long>::max();
   vector<long long> min_weight_list(L, INF);

   // 重み最小のノードを管理
   using WeightNode = pair<long long, int>;  // (startからの最小重み, ノード番号)
   priority_queue<WeightNode, vector<WeightNode>, greater<WeightNode>> node_queue;

   min_weight_list[start] = 0;
   node_queue.emplace(0, start);

   while (!node_queue.empty()) {
      const auto [min_weight, min_node] = node_queue.top();
      node_queue.pop();

      // すでに更新済みの場合はskip
      // - skipしないとO(N^2)となるケースが存在
      // see: https://snuke.hatenablog.com/entry/2021/02/22/102734
      if (min_weight_list[min_node] < min_weight) {
         continue;
      }

      // 重み最小のノードに隣接するノードを更新できるかチェック
      for (const auto& [node_to, weight] : adj_list[min_node]) {
         if (min_weight_list[node_to] > min_weight + weight) {
            min_weight_list[node_to] = min_weight + weight;
            node_queue.emplace(min_weight_list[node_to], node_to);
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
   vector<bool> visited(N + 1, false);
   constexpr long long INF = numeric_limits<long long>::max();

   int node = end;

   back_path.push_back(node);
   visited[node] = true;

   while (node != start) {
      for (auto [from, weight] : rev_adj_list[node]) {
         if (visited[from] || min_weight_list[from] == INF) continue;

         if (min_weight_list[node] == min_weight_list[from] + weight) {
            back_path.push_back(from);
            node = from;
            visited[from] = true;
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

int main() {
   int N, M, s, t;
   cin >> N >> M >> s >> t;

   s++;
   t++;

   // N: nodes, M: edges
   using Edge = pair<int, long long>;  // to, weight
   vector<vector<Edge>> adj_list(N + 1, vector<Edge>());

   for (int i = 0; i < M; i++) {
      int u, v;
      cin >> u >> v;

      long long w;
      cin >> w;

      u++;
      v++;
      adj_list[u].emplace_back(v, w);
   }

   ll INF = numeric_limits<ll>::max();
   auto min_dist = ShortestPathDijkstra(adj_list, s);

   if (min_dist[t] == INF) {
      cout << -1 << endl;
   } else {
      auto node_list = FindShortestPath(s, t, adj_list, min_dist);
      int E = node_list.size();

      cout << min_dist[t] << ' ' << E - 1 << endl;

      for (int i = 1; i < E; i++) {
         cout << (node_list[i - 1] - 1) << ' ' << (node_list[i] - 1);

         if (i == E - 1) {
            cout << endl;
         } else {
            cout << '\n';
         }
      }
   }

   return 0;
}