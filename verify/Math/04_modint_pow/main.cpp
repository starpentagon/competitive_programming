#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

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

using mint = ModIntDynamic;

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

int main() {
   mint::SetMod(1000000007LL);
   mint n, m;
   cin >> n >> m;

   mint ans = ipow(n, m.value());
   cout << ans << endl;
   return 0;
}