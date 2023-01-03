#include <unordered_set>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

// clang-format off
#define rep(i, n) for (int i = 0; (i) < (int)(n); (i)++)
 
template<class T> bool chmax(T &a, const T &b) {if(a<b) {a=b; return true;} return false; }
template<class T> bool chmin(T &a, const T &b) {if(a>b) {a=b; return true;} return false; }
 
template<class T> istream& operator>>(istream& is, vector<T>& vec){ rep(i, vec.size()) is >> vec[i]; return is;}
template<class T> ostream& operator<<(ostream& os, vector<T>& vec){ rep(i, vec.size()) os << vec[i] << (i+1==(int)vec.size() ? "" : " "); return os;}
// clang-format on

using ll = long long;
using ull = unsigned long long;

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
   const vector<pair<long long, long long>>& IntervalPrimeFactorization(long long N) const {
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

   long long M = (int)sqrt(N) + 10;
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

template <long long MOD>
class ModIntStatic {
  public:
   constexpr ModIntStatic(long long x = 0LL) noexcept {
      if (0 <= x && x < MOD) {
         x_ = x;
      } else {
         x_ = x % MOD;

         if (x_ < 0) {
            x_ += MOD;
         }
      }
   }

   // MODを返す
   constexpr long long GetMod() const {
      return MOD;
   }

   // MOD未満であることが保証されている値の場合、modを取らずにModIntStaticオブジェクトを生成する
   void SetRawValue(long long x) {
      assert(0 <= x && x < MOD);
      x_ = x;
   }

   static constexpr ModIntStatic raw(long long x) {
      ModIntStatic res;
      res.SetRawValue(x);

      return res;
   }

   // 値を返す
   constexpr long long value() const noexcept {
      return x_;
   }

   // 四則演算
   constexpr ModIntStatic operator+() const noexcept {
      return *this;
   }

   constexpr ModIntStatic operator-() const noexcept {
      return ModIntStatic(-x_);
   }

   constexpr ModIntStatic& operator+=(const ModIntStatic& rhs) noexcept {
      x_ += rhs.value();

      if (x_ >= MOD) {
         x_ -= MOD;
      }

      return *this;
   }

   constexpr ModIntStatic& operator++() noexcept {
      x_++;

      if (x_ == MOD) {
         x_ = 0;
      }

      return *this;
   }

   constexpr ModIntStatic operator++(int) noexcept {
      ModIntStatic res = *this;
      ++(*this);

      return res;
   }

   constexpr ModIntStatic& operator-=(const ModIntStatic& rhs) noexcept {
      x_ += MOD - rhs.value();

      if (x_ >= MOD) {
         x_ -= MOD;
      }

      return *this;
   }

   constexpr ModIntStatic& operator--() noexcept {
      if (x_ == 0) {
         x_ = MOD;
      }

      x_--;

      return *this;
   }

   constexpr ModIntStatic operator--(int) noexcept {
      ModIntStatic res = *this;
      --(*this);

      return res;
   }

   constexpr ModIntStatic& operator*=(const ModIntStatic& rhs) noexcept {
      x_ *= rhs.value();
      x_ %= MOD;

      return *this;
   }

   // 繰り返し二乗法でx^nを求める
   // 計算量: O(log n)
   // @note 0^0は不定値のためassertを入れている
   constexpr ModIntStatic pow(long long n) const;

   // 逆元を返す
   // @pre modが素数であること
   // 計算量: O(log P)
   constexpr ModIntStatic inv() const noexcept {
      return pow(MOD - 2);
   }

   constexpr ModIntStatic& operator/=(const ModIntStatic& rhs) noexcept {
      return (*this) *= rhs.inv();
   }

   constexpr ModIntStatic operator/(const ModIntStatic rhs) const noexcept {
      return ModIntStatic(*this) /= rhs;
   }

   friend ostream& operator<<(ostream& os, const ModIntStatic& rhs) {
      os << rhs.value();
      return os;
   }

   friend istream& operator>>(istream& is, ModIntStatic& rhs) {
      long long x;
      is >> x;
      rhs = x;
      return is;
   }

   friend constexpr ModIntStatic operator+(const ModIntStatic& lhs, const ModIntStatic& rhs) noexcept {
      return ModIntStatic(lhs) += rhs;
   }

   friend constexpr ModIntStatic operator-(const ModIntStatic& lhs, const ModIntStatic& rhs) noexcept {
      return ModIntStatic(lhs) -= rhs;
   }

   friend constexpr ModIntStatic operator*(const ModIntStatic& lhs, const ModIntStatic& rhs) noexcept {
      return ModIntStatic(lhs) *= rhs;
   }

   friend constexpr bool operator==(const ModIntStatic& lhs, const ModIntStatic& rhs) noexcept {
      return lhs.value() == rhs.value();
   }

   friend constexpr bool operator!=(const ModIntStatic& lhs, const ModIntStatic& rhs) noexcept {
      return lhs.value() != rhs.value();
   }

  private:
   long long x_;
};

template <long long MOD>
constexpr ModIntStatic<MOD> ModIntStatic<MOD>::pow(long long n) const {
   assert(!(x_ == 0 && n == 0));

   ModIntStatic<MOD> val(1);
   ModIntStatic<MOD> x(x_);

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

using mint = ModIntStatic<998244353>;
// using mint = ModIntStatic<1000000007>;

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

int main() {
   ll N, K;
   cin >> N >> K;
   PrimeTable prime_table;
   prime_table.IntervalBuild(N, K, true);

   // N!に現れる素数ごとに N! / (K! * (N-K)!)の指数を求める
   // 素数の集合としては[N-K+1, N]に現れる素因数を考えれば十分
   unordered_set<ll> prime_set;

   rep(i, K) {
      const auto& factor_list = prime_table.IntervalPrimeFactorization(N - i);

      for (auto [p, e] : factor_list) {
         prime_set.emplace(p);
      }
   }

   mint ans = 1;

   for (auto p : prime_set) {
      auto cnt1 = CalcExponentOfFactorial(p, N);
      auto cnt2 = CalcExponentOfFactorial(p, K);
      auto cnt3 = CalcExponentOfFactorial(p, N - K);

      auto cnt = cnt1 - cnt2 - cnt3;
      ans *= cnt + 1;
   }

   cout << ans << endl;

   return 0;
}