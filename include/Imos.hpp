#pragma once

#include <vector>
#include <iostream>

using namespace std;

// [Start] Imos 1D
// [Prefix] imos-1d-class
// [Verified] N, SetValue <= 10^5: The Maximum Number of Customers(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_5_A&lang=ja)
// 1次元のいもす法
template <class T>
class Imos {
  public:
   Imos(const size_t N);

   // 閉区間[l, r]に値を設定する(0-indexed)
   // @pre 0 <= l < N, l <= r(rはN以上でも可)
   void SetValue(const size_t l, const size_t r, const T& value);

   // 区間の累積値を明示的に計算する
   // @note []演算子でアクセスする際に自動で展開される。明示的に計算することを示す際に使用する。
   // 計算量: O(N)
   void Expand();

   // 累積値を取得する(0-indexed)
   // @note 取得はO(1)だが累積値が未計算の場合は自動で計算されO(N)かかる。
   const T& operator[](size_t i);

   // 要素数を返す
   size_t size() const {
      return data_.size();
   }

   // 展開前の配列を取得する
   const vector<T>& GetInOutArray() const {
      return in_out_;
   }

   // 展開後の配列を取得する
   // @note 自動でExpandは行わない
   const vector<T>& GetDataArray() const {
      if (!expand_) cerr << "Warning: not expanded" << endl;
      return data_;
   }

   friend ostream& operator<<(ostream& os, const Imos<T>& rhs) {
      auto vec = rhs.GetDataArray();
      os << vec;
      return os;
   }

   // @note 取得はO(1)だが累積値が未計算の場合は自動で計算されO(N)かかる。
   typename vector<T>::const_iterator begin() {
      if (!expand_) Expand();
      return data_.begin();
   }

   typename vector<T>::const_iterator end() {
      return data_.end();
   }

  private:
   size_t N_;          // 区間長
   bool expand_;       // 展開済かのフラグ
   vector<T> in_out_;  // 展開前のin/out情報を格納する
   vector<T> data_;    // 展開後のデータを格納する
};

template <class T>
Imos<T>::Imos(const size_t N)
    : N_(N), expand_(false), in_out_(N, 0), data_(N, 0) {
}

template <class T>
void Imos<T>::SetValue(const size_t l, const size_t r, const T& value) {
   assert(0 <= l && l < N_);
   assert(l <= r);

   expand_ = false;

   in_out_[l] += value;

   if (r + 1 < N_)
      in_out_[r + 1] += -value;
}

template <class T>
void Imos<T>::Expand() {
   if (expand_)
      return;

   expand_ = true;

   data_[0] = in_out_[0];

   for (size_t i = 1; i < N_; i++) {
      data_[i] = data_[i - 1] + in_out_[i];
   }
}

template <class T>
const T& Imos<T>::operator[](size_t i) {
   Expand();
   assert(0 <= i && i < N_);

   return data_[i];
}
// [End] Imos 1D

// [Start] Imos 2D
// [Prefix] imos-2d-class
// [Verified] H, W<=10^3, SetValue<=10^5: The Maximum Number of Overlaps(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_5_B&lang=ja)
// 2次元のいもす法
template <class T>
class Imos2D {
  public:
   // H行, W列のGridを生成する
   Imos2D(const size_t H, const size_t W);

   // 左上(i1, j1), 右下(i2, j2)(1-index, 閉領域)を頂点とする長方形領域に値を設定する
   // @pre 1 <= i1 <= H, 1 <= j1 <= W
   // @pre i1 <= i2, j1 <= j2
   void SetValue(const size_t i1, const size_t j1, const size_t i2, const size_t j2, const T& value);

   // 区間の累積値を明示的に計算する
   // @note []演算子でアクセスする際に自動で展開される。明示的に計算することを示す際に使用する。
   // 計算量: O(HW)
   void Expand();

   // 累積値を取得する(1-indexed)
   // @note 取得はO(1)だが累積値が未計算の場合は自動で計算されO(HW)かかる。
   // @pre 1 <= i <= H, 1 <= j <= W
   const T& get(const size_t i, const size_t j);

  private:
   size_t H_;                  // 行数
   size_t W_;                  // 列数
   bool expand_;               // 展開済かのフラグ
   vector<vector<T>> in_out_;  // 展開前のin/out情報を格納する
   vector<vector<T>> data_;    // 展開後のデータを格納する
};

template <class T>
Imos2D<T>::Imos2D(const size_t H, const size_t W)
    : H_(H), W_(W), expand_(false), in_out_(H + 1, vector<T>(W + 1, 0)), data_(H + 1, vector<T>(W + 1, 0)) {
}

template <class T>
void Imos2D<T>::SetValue(const size_t i1, const size_t j1, const size_t i2, const size_t j2, const T& value) {
   assert(1 <= i1 && i1 <= H_);
   assert(1 <= j1 && j1 <= W_);
   assert(i1 <= i2 && j1 <= j2);

   expand_ = false;

   in_out_[i1][j1] += value;

   if (j2 + 1 <= W_) {
      in_out_[i1][j2 + 1] += -value;
   }

   if (i2 + 1 <= H_) {
      in_out_[i2 + 1][j1] += -value;

      if (j2 + 1 <= W_) {
         in_out_[i2 + 1][j2 + 1] += value;
      }
   }
}

template <class T>
void Imos2D<T>::Expand() {
   if (expand_)
      return;

   expand_ = true;

   // 横方向の更新
   for (size_t i = 1; i <= H_; i++) {
      for (size_t j = 1; j <= W_; j++) {
         data_[i][j] = data_[i][j - 1] + in_out_[i][j];
      }
   }

   // 縦方向の更新
   for (size_t i = 2; i <= H_; i++) {
      for (size_t j = 1; j <= W_; j++) {
         data_[i][j] += data_[i - 1][j];
      }
   }
}

template <class T>
const T& Imos2D<T>::get(const size_t i, const size_t j) {
   Expand();
   assert(1 <= i && i <= H_);
   assert(1 <= j && j <= W_);

   return data_[i][j];
}
// [End] Imos 2D
