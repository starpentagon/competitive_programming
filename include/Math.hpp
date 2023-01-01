#pragma once

#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

// [Start] IsPrime
// [Prefix] is-prime-func
// 与えられた整数Nが素数かどうかを判定する
// 計算量: O(sqrt{N})
// [Verified] n<=10^8, ALDS1_1_C(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_1_C&lang=ja)
bool IsPrime(const long long N) {
   assert(N >= 1);

   if (N == 1) {
      return false;
   }

   for (long long i = 2; i * i <= N; i++) {
      if (N % i == 0) {
         return false;
      }
   }

   return true;
}
// [End] IsPrime

// [Start] Enumerate Divisors
// [Prefix] enum-divisors-func
// 正の整数Nのすべての約数を求める
// @param divisor_list : 約数の格納先
// 計算量 : O(sqrt{N})
// [Verified] n<=10^4, ITP1_3_D(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_3_D&lang=ja)
vector<long long> EnumerateDivisors(long long N) {
   assert(N >= 1);

   vector<long long> smalls, bigs;

   for (long long i = 1; i * i <= N; i++) {
      if (N % i != 0) {
         continue;
      }

      smalls.emplace_back(i);

      if (N / i != i) {
         bigs.emplace_back(N / i);
      }
   }

   vector<long long> divisor_list;
   divisor_list.reserve(smalls.size() + bigs.size());

   reverse(bigs.begin(), bigs.end());

   for (auto v : smalls) {
      divisor_list.emplace_back(v);
   }

   for (auto v : bigs) {
      divisor_list.emplace_back(v);
   }

   return divisor_list;
}
// [End] Enumerate Divisors

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
