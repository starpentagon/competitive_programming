#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#define debug(...) debug_print::multi_print(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) ;
#endif

// clang-format off
#define rep(i, n) for (int i = 0; (i) < (int)(n); (i)++)

template<class T> bool chmax(T &a, const T &b) {if(a<b) {a=b; return true;} return false; }
template<class T> bool chmin(T &a, const T &b) {if(a>b) {a=b; return true;} return false; }

template<class T> istream& operator>>(istream& is, vector<T>& vec){ rep(i, vec.size()) is >> vec[i]; return is;}
template<class T> ostream& operator<<(ostream& os, vector<T>& vec){ rep(i, vec.size()) os << vec[i] << (i+1==(int)vec.size() ? "" : " "); return os;}
// clang-format on

using ll = long long;
using ull = unsigned long long;

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

using mint = ModIntDynamic;

// 行列クラス
template <class T>
class Matrix {
  public:
   // M行N列の行列(要素=0で初期化)
   Matrix(const size_t M, const size_t N);

   // 2次元配列で初期化
   Matrix(const vector<vector<T>>& init);

   Matrix(const Matrix& matrix);

   // 行列のサイズを返す
   pair<size_t, size_t> size() const {
      return {M_, N_};
   }

   Matrix<T>& operator=(const Matrix<T>& matrix);

   // 繰り返し二乗法で累乗を計算する
   // @note n=0ならば単位行列を返す
   constexpr Matrix<T> pow(long long n) const;

   // 添え字演算子
   vector<T>& operator[](int i) {
      return matrix_[i];
   }

   const vector<T>& operator[](int i) const {
      return matrix_[i];
   }

   // 四則演算子
   constexpr Matrix<T> operator+() const {
      return *this;
   }

   constexpr Matrix<T> operator-() const;
   constexpr Matrix<T>& operator+=(const Matrix<T>& rhs);
   constexpr Matrix<T>& operator-=(const Matrix<T>& rhs);
   constexpr Matrix<T>& operator*=(const Matrix<T>& rhs);

  private:
   size_t M_;
   size_t N_;

   vector<vector<T>> matrix_;
};

template <class T>
Matrix<T>::Matrix(const size_t M, const size_t N)
    : M_(M), N_(N), matrix_(M, vector<T>(N, 0)) {
}

template <class T>
Matrix<T>::Matrix(const vector<vector<T>>& init)
    : M_(init.size()), N_(init[0].size()), matrix_(init) {
}

template <class T>
Matrix<T>::Matrix(const Matrix<T>& matrix) {
   M_ = matrix.size().first;
   N_ = matrix.size().second;

   matrix_.resize(M_);

   rep(i, M_) {
      matrix_[i].resize(N_);

      rep(j, N_) {
         matrix_[i][j] = matrix[i][j];
      }
   }
}

template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& matrix) {
   M_ = matrix.size().first;
   N_ = matrix.size().second;

   matrix_.resize(M_);

   rep(i, M_) {
      matrix_[i].resize(N_);

      rep(j, N_) {
         matrix_[i][j] = matrix[i][j];
      }
   }

   return *this;
}

template <class T>
constexpr Matrix<T> Matrix<T>::pow(long long n) const {
   assert(M_ == N_);

   Matrix<T> val(N_, N_);
   rep(i, N_) val[i][i] = 1;

   if (n == 0) return val;

   Matrix<T> X(matrix_);

   if (n < 0) {
      // 逆行列には未対応
      assert(n >= 0);
      // n = -n;
      // x = x.inv();
   }

   while (n > 0) {
      if ((n & 1LL) == 1) {
         // iビット目が1の場合、x^(2^i)をかける
         val *= X;
      }

      X *= X;
      n = n >> 1;
   }

   return val;
}

template <class T>
constexpr Matrix<T> Matrix<T>::operator-() const {
   Matrix matrix(matrix_);

   rep(i, M_) {
      rep(j, N_) {
         matrix[i][j] = -matrix[i][j];
      }
   }

   return matrix;
}

template <class T>
constexpr Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& rhs) {
   assert(rhs.size() == make_pair(M_, N_));

   rep(i, M_) {
      rep(j, N_) {
         matrix_[i][j] += rhs[i][j];
      }
   }

   return *this;
}

template <class T>
constexpr Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& rhs) {
   assert(rhs.size() == make_pair(M_, N_));

   rep(i, M_) {
      rep(j, N_) {
         matrix_[i][j] -= rhs[i][j];
      }
   }

   return *this;
}

template <class T>
constexpr Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& rhs) {
   assert(N_ == rhs.size().first);
   size_t K = rhs.size().second;

   vector<vector<T>> matrix(M_, vector<T>(K, 0));

   for (size_t k = 0; k < K; k++) {
      for (size_t i = 0; i < M_; i++) {
         for (size_t j = 0; j < N_; j++) {
            matrix[i][k] += matrix_[i][j] * rhs[j][k];
         }
      }
   }

   *this = matrix;
   return *this;
}

template <class T>
ostream& operator<<(ostream& os, const Matrix<T>& rhs) {
   auto [M, N] = rhs.size();

   rep(i, M) {
      os << "[ ";

      rep(j, N) {
         os << rhs[i][j] << ' ';
      }

      os << ']' << endl;
   }

   return os;
}

template <class T>
Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs) {
   Matrix<T> matrix(lhs);
   matrix += rhs;
   return matrix;
}

template <class T>
Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs) {
   Matrix<T> matrix(lhs);
   matrix -= rhs;
   return matrix;
}

template <class T>
Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs) {
   Matrix<T> matrix(lhs);
   matrix *= rhs;
   return matrix;
}

int main() {
   ll A, X, M;
   cin >> A >> X >> M;

   ModIntDynamic::SetMod(M);

   Matrix<mint> B(2, 2);
   B[0][0] = A;
   B[0][1] = 1;
   B[1][1] = 1;

   debug(B);

   auto C = B.pow(X - 1);
   // debug(C);
   auto ans = C[0][0] + C[0][1];

   cout << ans << endl;

   return 0;
}
