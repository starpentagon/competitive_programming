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

// 最大公約数を求める
// 計算量: O(log max(m, n))
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

// 最小公倍数を求める
// 計算量: O(log max(m, n))
// 依存ライブラリ: 最大公約数(GCM)
long long LCM(long long m, long long n) {
   assert(m > 0 && n > 0);

   long long gcd = GCD(m, n);
   return m / gcd * n;
}

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

int main() {
   using I = pair<Fraction, Fraction>;  // 区間

   int N;
   cin >> N;

   vector<I> il;

   rep(i, N) {
      ll x, y;
      cin >> x >> y;

      Fraction l1(y - 1, x, true);
      Fraction l2(y, x - 1, true);

      il.emplace_back(l2, l1);
   }

   sort(il.begin(), il.end());

   ll ans = 0;
   Fraction last(-1, 1, true);

   for (auto [l2, l1] : il) {
      if (l1 < last) continue;

      ans++;
      last = l2;
   }

   cout << ans << endl;

   return 0;
}