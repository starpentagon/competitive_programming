#pragma once

#include <cassert>
#include <iostream>
#include <limits>

using namespace std;

// [Start] ExtInt
// [Prefix] ext-int-class
// [Verified]
class ExtInt {
  public:
   constexpr ExtInt(long long x = 0LL) noexcept
       : x_(x) {
   }

   // 値を返す
   constexpr long long value() const noexcept {
      return x_;
   }

   // 四則演算
   constexpr ExtInt operator+() const noexcept {
      return *this;
   }

   // 符号の反転
   // @note NINF + 1は有限領域だが符号を反転するとINFになるのに注意
   constexpr ExtInt operator-() const noexcept {
      if (IsInf()) return ExtInt(NINF);
      if (IsNegativeInf()) return ExtInt(INF);

#ifdef LOCAL
      if (x_ == NINF + 1) {
         cerr << "val = NINF + 1(finite), but -val is now INF(infinite)." << endl;
      }
#endif

      return ExtInt(-x_);
   }

   constexpr ExtInt& operator+=(const ExtInt& rhs) noexcept {
      assert(!(IsInf() && rhs.IsNegativeInf()));
      assert(!(IsNegativeInf() && rhs.IsInf()));

      if (IsInf() || rhs.IsInf()) {
         x_ = INF;
      } else if (IsNegativeInf() || rhs.IsNegativeInf()) {
         x_ = NINF;
      } else {
         const auto v = rhs.value();

         if (v >= 0) {
            if (x_ >= INF - v) {
               // x + v >= INF
               x_ = INF;
            } else {
               // x + v < INF
               x_ += v;
            }
         } else {
            if (x_ <= NINF - v) {
               // x + v <= NINF
               x_ = NINF;
            } else {
               // x + v > NINF
               x_ += v;
            }
         }
      }

      return *this;
   }

   constexpr ExtInt& operator++() noexcept {
      if (IsInf() || IsNegativeInf()) return *this;

      x_++;
      return *this;
   }

   constexpr ExtInt operator++(int) noexcept {
      ExtInt res = *this;
      ++(*this);

      return res;
   }

   constexpr ExtInt& operator-=(const ExtInt& rhs) noexcept {
      *this += -rhs;
      return *this;
   }

   constexpr ExtInt& operator--() noexcept {
      if (IsInf() || IsNegativeInf()) return *this;

      x_--;
      return *this;
   }

   constexpr ExtInt operator--(int) noexcept {
      ExtInt res = *this;
      --(*this);

      return res;
   }

   constexpr ExtInt& operator*=(const ExtInt& rhs) noexcept {
      const long long v = rhs.value();

      if (IsInf()) {
         assert(v != 0);
         if (v < 0) x_ = NINF;
         return *this;
      }

      if (IsNegativeInf()) {
         assert(v != 0);
         if (v < 0) x_ = INF;
         return *this;
      }

      if (rhs.IsInf()) {
         assert(x_ != 0);

         if (x_ > 0)
            x_ = INF;
         else
            x_ = NINF;
         return *this;
      }

      if (rhs.IsNegativeInf()) {
         assert(x_ != 0);

         if (x_ > 0)
            x_ = NINF;
         else
            x_ = INF;
         return *this;
      }

      // x, v in (-inf, inf)
      if (x_ == 0 || v == 0) {
         x_ = 0;
         return *this;
      }

      // x, v in (-inf, inf), x != 0, v != 0
      bool positive_1 = x_ > 0;
      bool positive_2 = v > 0;

      if (positive_1 && positive_2) {
         // 同符号
         long long r = INF % abs(v);
         if (r > 0) r = 1;

         if (abs(x_) >= INF / abs(v) + r)
            x_ = INF;
         else
            x_ *= v;
      } else {
         // 異符号
         auto m = min(x_, v);  // 負の要素
         auto M = max(x_, v);  // 正の要素

         long long r = NINF % M;
         if (r < 0) r = -1;

         if (m <= NINF / M + r)
            x_ = NINF;
         else
            x_ *= v;
      }

      return *this;
   }

   constexpr ExtInt& operator/=(const ExtInt& rhs) noexcept {
      assert(IsFinite() || rhs.IsFinite());
      assert(rhs.value() != 0);

      if (rhs.IsInf() || rhs.IsNegativeInf()) {
         // x_: fininte
         return (*this) *= 0;
      }

      long long v = rhs.value();

      if (!IsFinite()) {
         if (v < 0)
            return (*this) = -(*this);
         else
            return *this;
      }

      return (*this) = x_ / rhs.value();
   }

   constexpr bool operator<(const ExtInt& rhs) const noexcept {
      return x_ < rhs.value();
   }

   constexpr ExtInt operator/(const ExtInt& rhs) const noexcept {
      return ExtInt(*this) /= rhs;
   }

   friend ostream& operator<<(ostream& os, const ExtInt& rhs) {
      os << rhs.value();
      return os;
   }

   friend istream& operator>>(istream& is, ExtInt& rhs) {
      long long x;
      is >> x;
      rhs = x;
      return is;
   }

   friend constexpr ExtInt operator+(const ExtInt& lhs, const ExtInt& rhs) noexcept {
      return ExtInt(lhs) += rhs;
   }

   friend constexpr ExtInt operator-(const ExtInt& lhs, const ExtInt& rhs) noexcept {
      return ExtInt(lhs) -= rhs;
   }

   friend constexpr ExtInt operator*(const ExtInt& lhs, const ExtInt& rhs) noexcept {
      return ExtInt(lhs) *= rhs;
   }

   friend constexpr bool operator==(const ExtInt& lhs, const ExtInt& rhs) noexcept {
      return lhs.value() == rhs.value();
   }

   friend constexpr bool operator!=(const ExtInt& lhs, const ExtInt& rhs) noexcept {
      return lhs.value() != rhs.value();
   }

   constexpr bool IsInf() const noexcept {
      return x_ == INF;
   }

   constexpr bool IsNegativeInf() const noexcept {
      return x_ == NINF;
   }

   constexpr bool IsFinite() const noexcept {
      if (IsInf() || IsNegativeInf()) return false;
      return true;
   }

   // 正の無限大(9,223,372,036,854,775,807)
   // @note INF != -NINFに注意
   static constexpr long long INF = numeric_limits<long long>::max();

   // 負の無限大(-9,223,372,036,854,775,808)
   // @note NINF != -INFに注意
   static constexpr long long NINF = numeric_limits<long long>::min();

  private:
   long long x_;
};

using eint = ExtInt;
// [ExtInt] ExtInt
