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

// 最長増加部分列を求める
// @param 数列
// @param max_len : max_lenが得られた時の部分列をsubsequenceに設定する
// @retval 最長増加部分列の長さ
template <class T = long long>
size_t LIS(const vector<T>& sequence, const size_t max_len = -1, vector<T>* subsequence = nullptr) {
   size_t N = sequence.size();

   // i回目の反復 = 要素0からi-1まで使う
   size_t len = 1;       // 最長増加部分列の長さ
   vector<T> dp(N + 1);  // dp[l]: 長さlの増加部分列の最後の要素の最小値

   dp[1] = sequence[0];

   for (size_t i = 1; i < N; i++) {
      const auto v = sequence[i];

      if (dp[len] < v) {
         // vを加えて増加列を延ばせる場合
         len += 1;
         dp[len] = v;

         if (max_len == len) {
            subsequence->clear();

            for (size_t j = 1; j <= len; j++) {
               subsequence->push_back(dp[j]);
            }
         }
      } else {
         // dp[1],...,dp[l](昇順になっている)の中でvを最初に超える要素をvで更新
         // vと同じ値がある場合は何もしない(実装上は同じ値で上書きされる)
         auto l = dp.begin() + 1;    // 長さ1
         auto r = dp.begin() + len;  // 長さlen
         auto it = lower_bound(l, r, v);

         *it = v;
      }
   }

   return len;
}

int main() {
   int N;
   cin >> N;

   vector<int> al(N);
   cin >> al;

   ll ans = LIS(al);
   cout << ans << endl;

   return 0;
}