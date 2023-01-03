#pragma once

#include <cmath>
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

// [Start] Prime enumeration and factorization(Sieve of Eratosthenes)
// [Prefix] prime-table-class
// [Verified] 素数列挙: N<=10^8, ALDS1_1_C(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_1_C&lang=ja)
// [Verified] 素因数分解: N<=10^3, UnitTest
// [Verified] 区間[N-K+1, N]の素数列挙, 素因数分解: N<=10^12, K<=10^6, ABC_227 「G - Divisors of Binomial Coefficient」(https://atcoder.jp/contests/abc227/tasks/abc227_g)
// エラトステネスの篩でN<=10^7もしくは区間[N-K+1, N](N<=10^14, K<=10^7)の
// - 素数列挙
// - 素因数分解
// を行う
class PrimeTable {
  public:
   PrimeTable();

   // 区間[1, N]の素数列挙を行う
   // 計算量: O(N log log N), 素因数分解を行う場合はN log N程度
   // 空間量: O(N)
   void Build(long long N, const bool factorization = false);

   // 素数判定
   // 計算量: O(1)
   bool IsPrime(long long N) const {
      assert(0 <= N && N <= N_);
      return prime_table_[N];
   }

   // 素因数分解
   // 計算量: O(1)
   vector<pair<long long, long long>> PrimeFactorization(long long N) const {
      assert(1 <= N && N <= N_);
      return prime_factor_[N];
   }

   // 区間[N-K+1, N]の素数列挙を行う
   // 計算量: O(K log log K + sqrt{N} log N), 素因数分解を行う場合はK log K程度
   // 空間量: O(K)
   void IntervalBuild(long long N, long long K, const bool factorization = false);

   // 区間[N-K+1, N]の素数判定
   // 計算量: O(1)
   bool IsIntervalPrime(long long N) const {
      long long A = interval_N_ - interval_K_ + 1;
      assert(A <= N && N <= interval_N_);
      return interval_prime_table_[N - A];
   }

   // 区間[N-K+1, N]の素因数分解
   // 計算量: O(1)
   vector<pair<long long, long long>> IntervalPrimeFactorization(long long N) const {
      long long A = interval_N_ - interval_K_ + 1;
      assert(A && N <= interval_N_);
      return interval_prime_factor_[N - A];
   }

  private:
   long long N_;  // 素数列挙の上限

   long long interval_N_;  // 素数の区間列挙の上限
   long long interval_K_;  // 素数の区間列挙の幅

   vector<bool> prime_table_;           // 素数フラグ
   vector<bool> interval_prime_table_;  // 区間[N-K+1, N]での素数フラグ

   vector<vector<pair<long long, long long>>> prime_factor_;           // prime_factor_[n]: nの素因数(p, e)リスト
   vector<vector<pair<long long, long long>>> interval_prime_factor_;  // interval_prime_factor_[n]: nの素因数(p, e)リスト
};

PrimeTable::PrimeTable()
    : N_(0), interval_N_(0), interval_K_(0) {
}

void PrimeTable::Build(long long N, const bool factorization) {
   N_ = N;
   prime_table_.clear();
   prime_table_.resize(N + 1, true);

   if (factorization) {
      prime_factor_.clear();
      prime_factor_.resize(N + 1);

      prime_factor_[1].emplace_back(2, 0);  // 1は2^0とする
   }

   prime_table_[0] = prime_table_[1] = false;

   auto calc_exponent = [](long long p, long long n) {
      long long cnt = 0;

      while (n >= p && (n % p) == 0) {
         cnt++;
         n /= p;
      }

      return cnt;
   };

   for (long long p = 2; p <= N; p++) {
      if (!prime_table_[p]) continue;

      // pは素数 -> pの倍数を合成数として記録する
      for (long long k = 2 * p; k <= N; k += p) {
         prime_table_[k] = false;

         if (factorization) {
            auto e = calc_exponent(p, k);
            prime_factor_[k].emplace_back(p, e);
         }
      }

      if (factorization) {
         prime_factor_[p].emplace_back(p, 1);
      }
   }
}

void PrimeTable::IntervalBuild(long long N, long long K, const bool factorization) {
   interval_N_ = N;
   interval_K_ = K;

   interval_prime_table_.clear();
   interval_prime_table_.resize(K, true);

   vector<long long> interval_prime_rest;
   long long A = N - K + 1;

   if (factorization) {
      interval_prime_factor_.clear();
      interval_prime_factor_.resize(K);

      interval_prime_rest.resize(K);
      for (int i = 0; i < K; i++) {
         interval_prime_rest[i] = A + i;
      }
   }

   long long M = (long long)sqrt(N) + 10;
   Build(M, false);

   for (long long p = 2; p * p <= N; p++) {
      if (!prime_table_[p]) continue;

      long long start = (A + p - 1) / p * p;
      if (start == p) start = 2 * p;

      for (long long k = start; k <= N; k += p) {
         interval_prime_table_[k - A] = false;

         if (factorization) {
            long long cnt = 0;

            while (interval_prime_rest[k - A] % p == 0) {
               interval_prime_rest[k - A] /= p;
               cnt++;
            }

            interval_prime_factor_[k - A].emplace_back(p, cnt);
         }
      }
   }

   if (factorization) {
      for (int k = 0; k < K; k++) {
         auto p = interval_prime_rest[k];

         if (p != 1) {
            interval_prime_factor_[k].emplace_back(p, 1);
         }
      }
   }
}

// [End] Prime enumeration and factorization(Sieve of Eratosthenes)

// [Start] Prime factorization of a specific N
// [Prefix] prime-factorization-func
// [Verified] n<=10^12, ABC_280「D - Factorial and Multiple」(https://atcoder.jp/contests/abc280/tasks/abc280_d)
// 素因数分解を行う
// @note N=prod p_i ^ k_iに分解し(p_i, k_i)のリストに記録する
// 計算量: O(sqrt{N})
vector<pair<long long, long long>> PrimeFactorization(long long N) {
   assert(N >= 2);

   vector<pair<long long, long long>> factorization;

   // sqrt{N}以下の数で分解
   for (long long p = 2; p * p <= N; p++) {
      if (N % p != 0) {
         continue;
      }

      // 割れる限り割り続ける
      long long n = 0;

      while (N % p == 0) {
         n++;
         N /= p;
      }

      factorization.emplace_back(p, n);
   }

   // 残りの因数pがあったとするとそれは素数でp^1の形になる。
   if (N != 1) {
      factorization.emplace_back(N, 1);
   }

   return factorization;
}
// [End] Prime factorization of a specific N

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
// [Verified] n<=10^12, ABC_280「D - Factorial and Multiple」(https://atcoder.jp/contests/abc280/tasks/abc280_d)
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
