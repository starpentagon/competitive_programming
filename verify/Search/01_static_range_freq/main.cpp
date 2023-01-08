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

// val以上で最小の要素を返す
// @note 条件を満たす要素がない場合はendイテレータを返す
// @note val_listはソート済であること
template <class T>
typename vector<T>::const_iterator find_min_greater_eq(const vector<T>& val_list, const T& val) {
   auto it = lower_bound(val_list.begin(), val_list.end(), val);
   return it;
}

// val以下で最大の要素を返す
// @note 条件を満たす要素がない場合はendイテレータを返す
// @note val_listはソート済であること
template <class T>
typename vector<T>::const_iterator find_max_less_eq(const vector<T>& val_list, const T& val) {
   // valより大きい要素のひとつ前の要素 = val以下で最大の要素
   auto it = upper_bound(val_list.begin(), val_list.end(), val);

   if (it == val_list.begin()) {
      return val_list.end();
   }

   --it;

   return it;
}

// valより小さい最大の要素を返す
// @note 条件を満たす要素がない場合はendイテレータを返す
// @note val_listはソート済であること
template <class T>
typename vector<T>::const_iterator find_max_less(const vector<T>& val_list, const T& val) {
   // val以上の要素のひとつ前の要素 = valより小さい最大の要素
   auto it = lower_bound(val_list.begin(), val_list.end(), val);

   if (it == val_list.begin()) {
      return val_list.end();
   }

   --it;

   return it;
}

int main() {
   map<int, vector<int>> val_index;

   int N, Q;
   cin >> N >> Q;

   rep(i, N) {
      int a;
      cin >> a;

      val_index[a].emplace_back(i);
   }

   rep(q, Q) {
      int l, r, x;
      cin >> l >> r >> x;

      int ans = 0;

      if (val_index.count(x) && l < r) {
         auto l_it = find_min_greater_eq(val_index[x], l);
         auto r_it = find_max_less(val_index[x], r);

         if (l_it != val_index[x].end() && r_it != val_index[x].end())
            ans = r_it - l_it + 1;
      }

      cout << ans << endl;
   }

   return 0;
}