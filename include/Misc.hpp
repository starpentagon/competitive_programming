#pragma once

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>

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

// [Start] Custom Hash of pair/tuple-vector-type
// [Prefix] custom-hash-struct
// 参考: pairをキーにしたstd::unordered_mapを手軽に使えるようにする(https://qiita.com/hamamu/items/4d081751b69aa3bb3557)
namespace custom_hash {
template <class T>
size_t HashCombine(const size_t seed, const T& v) {
   return seed ^ (std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}
template <typename T>
struct hash {
   size_t operator()(const T& keyval) const noexcept {
      return std::hash<T>()(keyval);
   }
};
/* pair用 */
template <class T, class S>
struct hash<std::pair<T, S>> {
   size_t operator()(const std::pair<T, S>& keyval) const noexcept {
      return HashCombine(std::hash<T>()(keyval.first), keyval.second);
   }
};
/* vector用 */
template <class T>
struct hash<std::vector<T>> {
   size_t operator()(const std::vector<T>& keyval) const noexcept {
      size_t s = 0;
      for (auto&& v : keyval) s = HashCombine(s, v);
      return s;
   }
};
/* tuple用 */
template <int N>
struct HashTupleCore {
   template <class Tuple>
   size_t operator()(const Tuple& keyval) const noexcept {
      size_t s = HashTupleCore<N - 1>()(keyval);
      return HashCombine(s, std::get<N - 1>(keyval));
   }
};
template <>
struct HashTupleCore<0> {
   template <class Tuple>
   size_t operator()(const Tuple& keyval) const noexcept {
      return 0;
   }
};
template <class... Args>
struct hash<std::tuple<Args...>> {
   size_t operator()(const std::tuple<Args...>& keyval) const noexcept {
      return HashTupleCore<std::tuple_size<std::tuple<Args...>>::value>()(keyval);
   }
};

template <typename K, typename T>
using unordered_map = std::unordered_map<K, T, hash<K>>;

}  // namespace custom_hash

// Note: main関数内に以下を追加すること
// using custom_hash::unordered_map;
// もしくは custom_hash::unordered_map<P, int> mp; と宣言する
// [End] Hash of pair-type

// [Start] Macros/Templates
// [Prefix] macro-templates
// clang-format off
#define rep(i, n) for (int i = 0; (i) < (int)(n); (i)++)
#define repp(i, s, n) for (int i = s; (i) < (int)(n); (i)++)

#define ALL(v) begin(v),end(v)
#define RALL(v) rbegin(v),rend(v)

template<class T> bool chmax(T &a, const T &b) {if(a<b) {a=b; return true;} return false; }
template<class T> bool chmin(T &a, const T &b) {if(a>b) {a=b; return true;} return false; }

template<class T> istream& operator>>(istream& is, vector<T>& vec){ rep(i, vec.size()) is >> vec[i]; return is;}
template<class T> ostream& operator<<(ostream& os, vector<T>& vec){ rep(i, vec.size()) os << vec[i] << (i+1==(int)vec.size() ? "" : " "); return os;}
// clang-format on

// [End] Macros/Templates

// [Start] Base Conversion
// [Prefix] base-conversion-class
// [Verified] 10進数 -> 2進数, N <= 10^18: ABC234「C - Happy New Year!」(https://atcoder.jp/contests/abc234/tasks/abc234_c)
class BaseConversion {
  public:
   BaseConversion(unsigned long long base_10_val);
   BaseConversion(int base, const vector<int>& digits);
   BaseConversion(int base, const string& digit_str);

   // 指定の基数で表現した文字列を返す
   string GetBaseStr(int base) const;

   // 指定の基数で表現した数値列を返す
   vector<int> GetBaseDigitList(int base) const;

   // 10進数での値を返す
   unsigned long long GetBase10Val() const;

  protected:
   unsigned long long base_10_val_;  // 10進数での値
};

BaseConversion::BaseConversion(unsigned long long base_10_val)
    : base_10_val_(base_10_val) {
}

BaseConversion::BaseConversion(int base, const vector<int>& digits)
    : base_10_val_(0) {
   assert(2 <= base && base <= 36);

   for (auto d : digits) {
      assert(0 <= d && d < base);
      base_10_val_ *= base;
      base_10_val_ += d;
   }
}

BaseConversion::BaseConversion(int base, const string& digit_str)
    : base_10_val_(0) {
   assert(2 <= base && base <= 36);

   for (auto c : digit_str) {
      int d = 0;
      if ('0' <= c && c <= '9') {
         d = c - '0';
      } else if ('A' <= c && c <= 'Z') {
         d = c - 'A' + 10;
      } else {
         assert(false);
      }

      assert(0 <= d && d < base);
      base_10_val_ *= base;
      base_10_val_ += d;
   }
}

vector<int> BaseConversion::GetBaseDigitList(int base) const {
   assert(2 <= base && base <= 36);
   vector<int> digits;

   if (base_10_val_ == 0) {
      digits.push_back(0);
      return digits;
   }

   auto val = base_10_val_;

   while (val > 0) {
      digits.push_back(val % base);
      val /= base;
   }

   reverse(digits.begin(), digits.end());
   return digits;
}

string BaseConversion::GetBaseStr(int base) const {
   auto digits = GetBaseDigitList(base);
   string ret;

   for (auto d : digits) {
      if (d < 10) {
         ret += '0' + d;
      } else {
         ret += 'A' + d - 10;
      }
   }

   return ret;
}

unsigned long long BaseConversion::GetBase10Val() const {
   return base_10_val_;
}

// [End] Base Conversion
