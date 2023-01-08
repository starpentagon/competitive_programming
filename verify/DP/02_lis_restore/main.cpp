#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <map>

using namespace std;

#ifdef LOCAL
#include <bits/stdc++.h>
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

// 最長増加部分列(LIS)を求める
// @param vals: LISを求める数列
// @retval LIS
template <class T = long long>
vector<T> LIS(const vector<T>& vals) {
   size_t N = vals.size();

   // i回目の反復 = 要素0からi-1まで使う
   size_t len = 1;          // 最長増加部分列の長さ
   using P = pair<T, int>;  // (val, index)

   vector<P> dp(N + 1, P(0, -1));  // dp[l]: 長さlの増加部分列の最後の要素の最小値
   vector<int> pre_pos(N, -1);     // 復元用テーブル: dp[l]を更新した際のdp[l-1]の最小要素の位置を持つ

   dp[len] = P(vals[0], 0);

   for (size_t i = 1; i < N; i++) {
      const auto v = vals[i];

      if (dp[len].first < v) {
         // vを加えて増加列を延ばせる場合
         len += 1;
         dp[len] = P(v, i);
         pre_pos[i] = dp[len - 1].second;
      } else {
         // dp[len] >= v
         // dp[1],...,dp[len](昇順になっている)の中でvを最初に超える要素をvで更新
         auto l = dp.begin() + 1;    // 長さ1
         auto r = dp.begin() + len;  // 長さlen
         auto it = lower_bound(l, r, P(v, -1));

         int update = it - l + 1;
         dp[update] = P(v, i);
         pre_pos[i] = dp[update - 1].second;
      }
   }

   vector<T> lis;
   lis.reserve(len);

   int i = dp[len].second;

   while (true) {
      lis.emplace_back(vals[i]);

      i = pre_pos[i];
      if (i == -1) break;
   }

   reverse(lis.begin(), lis.end());
   return lis;
}

int main() {
   int N;
   cin >> N;

   vector<int> al(N);
   cin >> al;

   auto lis = LIS(al);

   cout << lis.size() << endl;

   auto find_index = [&](int v, int start) {
      for (int i = start; i < N; i++) {
         if (al[i] == v) {
            return i;
         }
      }
      return -1;
   };

   int i = 0;
   vector<int> ans;

   for (auto v : lis) {
      i = find_index(v, i);
      assert(i != -1);
      ans.emplace_back(i);
   }

   cout << ans << endl;

   return 0;
}