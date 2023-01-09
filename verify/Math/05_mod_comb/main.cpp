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

// using mint = ModIntStatic<998244353>;
using mint = ModIntStatic<1000000007>;

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
    : N_(max(1, N)), factorial_(N_ + 1, 0), inversion_(N_ + 1, 0), fact_inv_(N_ + 1, 0) {
   factorial_[0] = factorial_[1] = 1;
   inversion_[1] = 1;
   fact_inv_[0] = fact_inv_[1] = 1;

   static const long long MOD = T().GetMod();
   assert(MOD > N_);

   for (int i = 2; i <= N_; i++) {
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

int main() {
   int N, K;
   cin >> N >> K;

   vector<int> al(N);
   cin >> al;

   CombModTable<mint> comb_tbl(N);

   mint ans = 0;

   sort(al.begin(), al.end());

   rep(i, N) {
      auto a = al[i];
      ans += a * comb_tbl.get(i, K - 1);
   }

   sort(al.rbegin(), al.rend());

   rep(i, N) {
      auto a = al[i];
      ans -= a * comb_tbl.get(i, K - 1);
   }

   cout << ans << endl;
   return 0;
}