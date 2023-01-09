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

int main() {
   int N, M, K;
   cin >> N >> M >> K;

   Matrix<mint> A(N, M), B(M, K);

   rep(i, N) rep(j, M) cin >> A[i][j];
   rep(i, M) rep(j, K) cin >> B[i][j];

   auto C = A * B;

   rep(i, N) rep(j, K) {
      cout << C[i][j];

      if (j == K - 1) {
         cout << endl;
      } else {
         cout << ' ';
      }
   }

   return 0;
}