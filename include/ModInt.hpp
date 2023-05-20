#pragma once

#include <cassert>
#include <iostream>

using namespace std;

// [Start] ModInt(static)
// [Prefix] mod-int-static-class
// [Verified] n^m, n<=100, m<=10^9, NTL_1_B(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=NTL_1_B&lang=ja)
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

   constexpr ModIntStatic operator/(const ModIntStatic& rhs) const noexcept {
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

   if (n < 0) {
      n = -n;
      x = x.inv();
   }

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
// using mint = ModIntStatic<1000000007>;
// [End] ModInt(static)

// [Start] ModInt(dynamic)
// [Prefix] mod-int-dynamic-class
// [Verified] n^m, n<=100, m<=10^9, NTL_1_B(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=NTL_1_B&lang=ja)
class ModIntDynamic {
  public:
   ModIntDynamic(long long x = 0LL) {
      if (0 <= x && x < mod_) {
         x_ = x;
      } else {
         x_ = x % mod_;

         if (x_ < 0) {
            x_ += mod_;
         }
      }
   }

   static long long GetMod() {
      return mod_;
   }

   static void SetMod(long long mod) {
      mod_ = mod;
   }

   // MOD未満であることが保証されている値の場合、modを取らずにModIntDynamicオブジェクトを生成する
   void SetRawValue(long long x) {
      assert(0 <= x && x < mod_);
      x_ = x;
   }

   static ModIntDynamic raw(long long x) {
      ModIntDynamic res;
      res.SetRawValue(x);

      return res;
   }

   // 値を返す
   long long value() const noexcept {
      return x_;
   }

   // 四則演算
   ModIntDynamic operator+() const {
      return *this;
   }

   ModIntDynamic operator-() const {
      return ModIntDynamic(-x_);
   }

   ModIntDynamic& operator+=(const ModIntDynamic& rhs) {
      x_ += rhs.value();

      if (x_ >= mod_) {
         x_ -= mod_;
      }

      return *this;
   }

   ModIntDynamic& operator++() {
      x_++;

      if (x_ == mod_) {
         x_ = 0;
      }

      return *this;
   }

   ModIntDynamic operator++(int) {
      ModIntDynamic res = *this;
      ++(*this);

      return res;
   }

   ModIntDynamic& operator-=(const ModIntDynamic& rhs) {
      x_ += mod_ - rhs.value();

      if (x_ >= mod_) {
         x_ -= mod_;
      }

      return *this;
   }

   ModIntDynamic& operator--() {
      if (x_ == 0) {
         x_ = mod_;
      }

      x_--;

      return *this;
   }

   ModIntDynamic operator--(int) {
      ModIntDynamic res = *this;
      --(*this);

      return res;
   }

   ModIntDynamic& operator*=(const ModIntDynamic& rhs) {
      x_ *= rhs.value();
      x_ %= mod_;

      return *this;
   }

   // 繰り返し二乗法でx^nを求める
   // 計算量: O(log n)
   // @note 0^0は不定値のためassertを入れている
   ModIntDynamic pow(long long n) const;

   // 逆元を返す
   // @pre modが素数であること
   // 計算量: O(log P)
   ModIntDynamic inv() const {
      return pow(mod_ - 2);
   }

   ModIntDynamic& operator/=(const ModIntDynamic& rhs) {
      return (*this) *= rhs.inv();
   }

   ModIntDynamic operator/(const ModIntDynamic rhs) const {
      return ModIntDynamic(*this) /= rhs;
   }

   friend ostream& operator<<(ostream& os, const ModIntDynamic& rhs) {
      os << rhs.value();
      return os;
   }

   friend istream& operator>>(istream& is, ModIntDynamic& rhs) {
      long long x;
      is >> x;
      rhs = x;
      return is;
   }

   friend ModIntDynamic operator+(const ModIntDynamic& lhs, const ModIntDynamic& rhs) {
      return ModIntDynamic(lhs) += rhs;
   }

   friend ModIntDynamic operator-(const ModIntDynamic& lhs, const ModIntDynamic& rhs) {
      return ModIntDynamic(lhs) -= rhs;
   }

   friend ModIntDynamic operator*(const ModIntDynamic& lhs, const ModIntDynamic& rhs) {
      return ModIntDynamic(lhs) *= rhs;
   }

   friend bool operator==(const ModIntDynamic& lhs, const ModIntDynamic& rhs) {
      return lhs.value() == rhs.value();
   }

   friend bool operator!=(const ModIntDynamic& lhs, const ModIntDynamic& rhs) {
      return lhs.value() != rhs.value();
   }

  private:
   long long x_;
   static long long mod_;
};

// 法の定義（実行時に与えられる場合はSetMod関数で設定する）
// @pre mod は31bitの範囲に収まる(積が63bitに収まる)素数であること
long long ModIntDynamic::mod_ = 998244353LL;

ModIntDynamic ModIntDynamic::pow(long long n) const {
   assert(!(x_ == 0 && n == 0));

   ModIntDynamic val(1);
   ModIntDynamic x(x_);

   if (n < 0) {
      n = -n;
      x = x.inv();
   }

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

// using mint = ModIntDynamic;
//  [End] ModInt(dynamic)
