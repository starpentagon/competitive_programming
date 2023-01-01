#pragma once

#include <cassert>
#include <vector>
#include <algorithm>
#include <ostream>

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

// [Start] Power of n
// [Prefix] pow-func
// [Verified] n^m, n<=100, m<=10^9, NTL_1_B(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=NTL_1_B&lang=ja)
// 繰り返し二乗法でx^nを求める
// 計算量: O(log n)
// @note 0^0は不定値のためassertを入れている
template <class T>
T ipow(T x, long long n) {
   assert(!(x == 0 && n == 0));

   T val = 1;

   while (n > 0) {
      if ((n & 1LL) == 1) {
         // iビット目が1の場合、x^(2^i)をかける
         val *= x;
      }

      x *= x;
      n = n >> 1;
   }

   return val;
}
// [End] Power of n

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

// [Start] Combination(nCk)
// [Prefix] comb-dp-class
// 組合せ数 nCk を求める(DP版)
// 時間量, 空間量ともにO(N^2)のため N <= 2 * 10^3程度であれば適用可能
class CombTable {
  public:
   CombTable(const int N);

   long long get(const int n, const int k) const;

  private:
   const int N_;  // 計算する最大のN
   vector<vector<long long>> table_;
};

CombTable::CombTable(const int N)
    : N_(N), table_(N + 1, vector<long long>(N + 1, 0)) {
   table_[0][0] = 1;

   for (int i = 1; i <= N; i++) {
      table_[i][0] = 1;

      for (int j = 1; j <= i; j++) {
         table_[i][j] = table_[i - 1][j - 1];
         table_[i][j] += table_[i - 1][j];
      }
   }
}

long long CombTable::get(const int n, const int k) const {
   if (!(0 <= k && k <= n))
      return 0;

   assert(n <= N_);

   // 0 <= k <= n <= N
   return table_[n][k];
}

// [End] Combination(nCk)

// [Start] Mod Combination(nCk mod p)
// [Prefix] comb-mod-class
// [Verified] n<=10^5, ABC_151「E - Max-Min Sums」(https://atcoder.jp/contests/abc151/tasks/abc151_e)
// 組合せ数 nCk を求める(Mod版)
// 時間量, 空間量: O(N), N <= 10^7程度まで適用可能
// @pre Modが素数でMod > nであること
template <class T>
class CombModTable {
  public:
   CombModTable(const int N);

   T get(const int n, const int k) const;

  private:
   int N_;                // 計算する最大のN
   vector<T> factorial_;  // n!
   vector<T> inversion_;  // 逆元の累積積
   vector<T> fact_inv_;   // (k!)^-1
};

template <class T>
CombModTable<T>::CombModTable(const int N)
    : N_(N), factorial_(N + 1, 0), inversion_(N + 1, 0), fact_inv_(N + 1, 0) {
   assert(N >= 1);
   factorial_[0] = factorial_[1] = 1;
   inversion_[1] = 1;
   fact_inv_[0] = fact_inv_[1] = 1;

   static const long long MOD = T().GetMod();
   assert(MOD > N);

   for (int i = 2; i <= N; i++) {
      factorial_[i] = factorial_[i - 1];
      factorial_[i] *= i;

      inversion_[i] = MOD - inversion_[MOD % i].value() * (MOD / i) % MOD;

      fact_inv_[i] = fact_inv_[i - 1];
      fact_inv_[i] *= inversion_[i];
   }
}

template <class T>
T CombModTable<T>::get(const int n, const int k) const {
   if (!(0 <= k && k <= n))
      return 0;

   assert(n <= N_);

   // 0 <= k <= n <= N
   T ret = factorial_[n];
   ret *= fact_inv_[k];
   ret *= fact_inv_[n - k];

   return ret;
}
// [End] Mod Combination(nCk mod p)

// [Start] Fraction
// [Prefix] fraction-class
// [Verified] 比較演算子, n<=2*10^5, ABC_225「E - 7」(https://atcoder.jp/contests/abc225/tasks/abc225_e)
// 有理数クラス
// 依存ライブラリ: 最大公約数(GCM), 最小公倍数(LCM)
class Fraction {
  public:
   // @param numerator 分子
   // @param denominator 分母
   // @param reduce 通分するか(default: false), 通分する場合は計算量: O(log min(num, den))
   Fraction(const long long numerator, const long long denominator, const bool reduce = false)
       : n_(numerator), d_(denominator), reduce_(reduce) {
      // 分母 >= 0にする(比較演算子で不等号の向きが変わらないようにするため)
      if (denominator == 0) {
         n_ = 1;
         d_ = 0;
         return;
      } else if (denominator < 0) {
         n_ = -numerator;
         d_ = -denominator;
      }

      if (reduce) {
         long long gcd = GCD(abs(n_), d_);

         n_ /= gcd;
         d_ /= gcd;
      }
   }

   Fraction(const Fraction& f)
       : n_(f.get_num()), d_(f.get_denom()), reduce_(f.reduce_) {
   }

   // 値を返す(double型に変換)
   double value_dbl() const noexcept {
      return (double)n_ / d_;
   }

   // 分子, 分母のpair型を取得する
   pair<long long, long long> get_num_denom() const noexcept {
      return {n_, d_};
   }

   // 分子を取得する
   long long get_num() const noexcept {
      return n_;
   }

   // 分母を取得する
   long long get_denom() const noexcept {
      return d_;
   }

   // 比較演算子
   // @note 分子・分母の掛け算が発生するため分子、分母がint型の範囲でないとオーバーフローの可能性あり
   friend constexpr bool operator<(const Fraction& lhs, const Fraction& rhs) noexcept {
      return lhs.n_ * rhs.d_ < rhs.n_ * lhs.d_;
   }

   friend bool operator==(const Fraction& lhs, const Fraction& rhs) {
      return lhs.n_ * rhs.d_ == rhs.n_ * lhs.d_;
   }

   friend bool operator!=(const Fraction& lhs, const Fraction& rhs) {
      return !(lhs == rhs);
   }

   // 四則演算子
   Fraction operator+() const {
      return *this;
   }

   Fraction operator-() const {
      auto [n, d] = get_num_denom();
      return Fraction(-n, d);
   }

   Fraction& operator+=(const Fraction& rhs) {
      auto [n, d] = rhs.get_num_denom();

      auto lcm = LCM(d, d_);

      n_ = n_ * (lcm / d_) + n * (lcm / d);
      d_ = lcm;

      if (reduce_) {
         auto gcd = GCD(abs(n_), d_);

         n_ /= gcd;
         d_ /= gcd;
      }

      return *this;
   }

   Fraction& operator-=(const Fraction& rhs) {
      *this += -rhs;
      return *this;
   }

   Fraction& operator*=(const Fraction& rhs) {
      auto [n, d] = rhs.get_num_denom();

      if (reduce_ && !rhs.reduce_) {
         auto gcd = GCD(abs(n), d);
         n /= gcd;
         d /= gcd;
      }

      n_ *= n;
      d_ *= d;

      if (d_ == 0) {
         n_ = 1;
      }

      return *this;
   }

   Fraction& operator/=(const Fraction& rhs) {
      auto [n, d] = rhs.get_num_denom();
      *this *= Fraction(d, n);

      return *this;
   }

   friend Fraction operator+(const Fraction& lhs, const Fraction& rhs) {
      return Fraction(lhs) += rhs;
   }

   friend Fraction operator-(const Fraction& lhs, const Fraction& rhs) {
      return Fraction(lhs) -= rhs;
   }

   friend Fraction operator*(const Fraction& lhs, const Fraction& rhs) {
      return Fraction(lhs) *= rhs;
   }

   friend Fraction operator/(const Fraction& lhs, const Fraction& rhs) {
      return Fraction(lhs) /= rhs;
   }

   friend ostream& operator<<(ostream& os, const Fraction& rhs) {
      auto [n, d] = rhs.get_num_denom();
      auto v = rhs.value_dbl();

      os << n << " / " << d << " = " << v;
      return os;
   }

  protected:
   long long n_;  // 分子
   long long d_;  // 分母
   bool reduce_;  // 通分フラグ
};
// [End] Fraction

// [Start] exponent of N
// [Prefix] exponent-N-func
// Nの素数pの指数部を求める
// @note 計算量: O(log N), Nの素因数の指数部の合計がlog Nで抑えられる
// Nの素数pの指数部を求める
// @note 計算量: O(log N), Nの素因数の指数部の合計がlog Nで抑えられる
long long CalcExponentOfN(long long p, long long N) {
   assert(p >= 2);
   long long cnt = 0;

   while (N >= p && (N % p) == 0) {
      cnt++;
      N /= p;
   }

   return cnt;
}
// [End] exponent of N

// [Start] exponent of factorial
// [Prefix] exponent-factorial-func
// ToDo: 素因数分解を実装後にVerify https://atcoder.jp/contests/abc280/tasks/abc280_d
// N!の素数pの指数部を求める
// @note ルジャンドルの定理に基づく
// @note 計算量: O(log N)
long long CalcExponentOfFactorial(long long p, long long N) {
   assert(p >= 2);
   long long cnt = 0;
   long long q = p;

   while (N >= q) {
      cnt += N / q;

      if (N / q < p) {
         break;
      }

      q *= p;
   }

   return cnt;
}
// [End] exponent of factorial
