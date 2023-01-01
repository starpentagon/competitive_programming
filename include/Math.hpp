#pragma once

#include <cassert>

// [Start] GCD
// [Prefix] gcd-func
// 最大公約数を求める
// 計算量: O(log max(m, n))
// [Verified] m,n<=10^9, ALDS1_1_B(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_1_B&lang=ja)
long long GCD(long long m, long long n) {
   assert(m >= 0 && n >= 0);

   if (m < n) return GCD(n, m);
   // m >= n
   if (n == 0) return m;

   long long r = 0;

   while ((r = m % n)) {
      m = n;
      n = r;
   }

   return n;
}
// [End] GCD

// [Start] LCM
// [Prefix] lcm-func
// 最小公倍数を求める
// 計算量: O(log max(m, n))
// 依存ライブラリ: 最大公約数(GCM)
long long LCM(long long m, long long n) {
   assert(m > 0 && n > 0);

   long long gcd = GCD(m, n);
   return m / gcd * n;
}
// [End] LCM
