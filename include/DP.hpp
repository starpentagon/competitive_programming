#pragma once

#include <vector>
using namespace std;

void DPFramework() {
   int N = 0;
   long long __DUMMY__ = 0;

   // [Start] DP Framework
   // [Prefix] dp-framework-inline
   // 状態を宣言
   // 状態の定義: dp[i][j] =
   long long INF = __DUMMY__;  // 到達不可能な状態
   vector<vector<long long>> dp(N + 1, vector<long long>(N + 1, INF));

   // 初期化

   for (int i = 1; i <= N; i++) {
      for (int j = 1; j <= N; j++) {
         // 到達可能かチェック
         if (dp[i][j] == INF) {
            continue;
         }

         // 状態の更新
      }
   }

   // 答えを求める

   // [End] DP Framework
}

// [Start] LIS: Longest Increasing subsequence
// [Prefix] dp-lis-func
// [Verified] N<=10^5: Longest Increasing Subsequence(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_1_D&lang=ja)
// [Verified] N<=5*10^5: Longest Increasing Subsequence(https://judge.yosupo.jp/problem/longest_increasing_subsequence)
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
// [End] LIS: Longest Increasing subsequence
