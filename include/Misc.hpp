#pragma once

#include <vector>

using namespace std;

// [Start] Position Compression
// [Prefix] pos-comp-class
// [Verified] N<=10^5: ABC_036「C - 座圧」(https://atcoder.jp/contests/abc036/tasks/abc036_c)
template <class T>
class PosCompression {
  public:
   // dataを圧縮する
   // 計算量: O(N log N)
   PosCompression(const vector<T>& data);

   PosCompression() {
   }

   // 圧縮前の値valを圧縮した値を返す
   // @pre valが登録されていること
   size_t Encode(const T& val) const;

   // 圧縮後の値indexの圧縮前の値を返す
   T Decode(const size_t index) const;

   // 要素数を返す
   size_t size() const {
      return comp_.size();
   }

  private:
   vector<T> comp_;  // comp_[index]: dataのユニークな値を昇順ソートした時にindex番目の値
};

template <class T>
PosCompression<T>::PosCompression(const vector<T>& data)
    : comp_(data) {
   // 昇順ソートし重複を削除する
   sort(comp_.begin(), comp_.end());
   comp_.erase(unique(comp_.begin(), comp_.end()), comp_.end());
}

template <class T>
size_t PosCompression<T>::Encode(const T& val) const {
   size_t index = lower_bound(comp_.begin(), comp_.end(), val) - comp_.begin();
   return index;
}

template <class T>
T PosCompression<T>::Decode(const size_t index) const {
   assert(0 <= index && index < comp_.size());
   return comp_[index];
}
// [End] Position Compression
