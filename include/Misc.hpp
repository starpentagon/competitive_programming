#pragma once

#include <iostream>
#include <iomanip>
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

// [Start] Run length encoding
// [Prefix] run-len-func
// [Verified] N <= 2 * 10^5, ABC299「C - Dango」
// (value, count)型にランレングス圧縮を行う
template <class T>
vector<pair<T, long long>> RunLengthEncoding(const vector<T>& vec) {
   vector<pair<T, long long>> encode;

   for (auto v : vec) {
      if (encode.empty() || encode.back().first != v) {
         encode.emplace_back(v, 1);
      } else {
         encode.back().second++;
      }
   }

   return encode;
}
// [End] Run length encoding

void Cout() {
   // clang-format off

   // [Start] cout(float)
   // [Prefix] cout-float-inline
cout << fixed << setprecision(15);
   // [End] cout(float)

   int ans = 0;

   // [Start] print ans
   // [Prefix] p-ans-inline
cout << ans << endl;
   // [End] print ans

   // [Start] print Yes
   // [Prefix] p-yes-inline
cout << "Yes" << endl;
   // [End] print Yes

   // [Start] print No
   // [Prefix] p-no-inline
cout << "No" << endl;
   // [End] print No

   // [Start] print -1
   // [Prefix] p-neg-inline
cout << -1 << endl;
   // [End] print -1
   // clang-format on
}

// [Start] Hash of pair-type
// [Prefix] hash-pair-struct
struct HashPair {
   template <class T1, class T2>
   size_t operator()(const pair<T1, T2>& p) const {
      auto hash1 = hash<T1>{}(p.first);
      auto hash2 = hash<T2>{}(p.second);

      // 重複しないようにハッシュ処理
      size_t seed = 123456789;
      seed ^= hash1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      seed ^= hash2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      return seed;
   }
};
// [End] Hash of pair-type

// [Start] Macros/Templates
// [Prefix] macro-templates
// clang-format off
#define rep(i, n) for (int i = 0; (i) < (int)(n); (i)++)

template<class T> bool chmax(T &a, const T &b) {if(a<b) {a=b; return true;} return false; }
template<class T> bool chmin(T &a, const T &b) {if(a>b) {a=b; return true;} return false; }

template<class T> istream& operator>>(istream& is, vector<T>& vec){ rep(i, vec.size()) is >> vec[i]; return is;}
template<class T> ostream& operator<<(ostream& os, vector<T>& vec){ rep(i, vec.size()) os << vec[i] << (i+1==(int)vec.size() ? "" : " "); return os;}
// clang-format on

// [End] Macros/Templates
