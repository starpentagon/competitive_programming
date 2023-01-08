#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <map>

using namespace std;

#ifdef LOCAL
#include <bits/stdc++.h>
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

template <class T>
class PosCompression {
  public:
   PosCompression() {
   }
   // dataを圧縮する
   PosCompression(const vector<T>& data);

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

int main() {
   int N;
   cin >> N;

   vector<int> al(N);
   cin >> al;

   PosCompression comp(al);

   vector<int> bl;

   for (auto a : al) {
      int b = comp.Encode(a);
      bl.emplace_back(b);
   }

   cout << bl << endl;

   return 0;
}