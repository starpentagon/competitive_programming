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

int main() {
   int n, m, l;
   cin >> n >> m >> l;

   vector<vector<ll>> al;
   vector<vector<ll>> bl;

   rep(i, n) {
      vector<ll> v(m);
      cin >> v;
      al.emplace_back(v);
   }

   rep(i, m) {
      vector<ll> v(l);
      cin >> v;
      bl.emplace_back(v);
   }

   Matrix a_mat(al);
   Matrix b_mat(bl);

   auto c_mat = a_mat * b_mat;

   rep(i, n) {
      cout << c_mat[i] << endl;
   }

   return 0;
}