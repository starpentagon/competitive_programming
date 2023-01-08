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
// 最長増加部分列を求める
// @param sequence: 数列
// @param max_len : max_lenが得られた時の部分列をsubsequenceに設定する
// @retval 最長増加部分列の長さ
// @note 計算量: O(N log N)
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
// [End] LIS: Longest Increasing subsequence
