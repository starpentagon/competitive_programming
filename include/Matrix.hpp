#pragma once

#include <vector>
#include <iostream>

using namespace std;

// [Start] Matrix
// [Prefix] matrix-class
// [Verified] 行列積: n, m<=10^2, ITP1_7_D(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_7_D&lang=ja)
// [Verified] 行列積: n, m<=10^3, Matrix Product(https://judge.yosupo.jp/problem/matrix_product)
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
// [End] Matrix
