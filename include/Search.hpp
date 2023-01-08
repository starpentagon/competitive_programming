#pragma once

#include <vector>
#include <set>

using namespace std;

void BinarySearchInt() {
   long long __DUMMY__ = 0;

   // [Start] Binary Search(Int)
   // [Prefix] bin-search-int-inline
   // 条件を満たす境界(最小/最大)を二分探索で求める。
   // kTrivialOK: 条件を満たす値, kTrivialNG: 条件を満たさない値
   // 計算量: O(log(|kTrivialOK - kTrivialNG|))
   long long kTrivialOK = __DUMMY__;
   long long kTrivialNG = __DUMMY__;

   auto is_ok = [&](long long v) -> bool {
      // 条件判定を実装
   };

   long long ok = kTrivialOK, ng = kTrivialNG;

   auto bin_search_finish = [](long long ok, long long ng) {
      if (ok > ng && ok - 1 == ng) {
         return true;
      }
      if (ok < ng && ok == ng - 1) {
         return true;
      }
      return false;
   };

   while (!bin_search_finish(ok, ng)) {
      long long m = ok / 2 + ng / 2;
      m += ((ok % 2) + (ng % 2)) / 2;

      if (is_ok(m)) {
         ok = m;
      } else {
         ng = m;
      }
   }

   // 変数okが条件を満たす最大/最小になっている

   // [End] Binary Search(Int)
}

void BinarySearchDbl() {
   double __DUMMY__ = 0;
   // [Start] Binary Search(Double)
   // [Prefix] bin-search-dbl-inline
   // 条件を満たす境界(最小/最大)を二分探索で求める。
   // kTrivialOK: 条件を満たす値, kTrivialNG: 条件を満たさない値
   // 計算量: O(log(|kTrivialOK - kTrivialNG|))
   double kTrivialOK = __DUMMY__;
   double kTrivialNG = __DUMMY__;

   auto is_ok = [&](double v) -> bool {
      // 条件判定を実装
   };

   double ok = kTrivialOK, ng = kTrivialNG;

   for (int i = 0; i < 100; i++) {
      double m = (ok + ng) / 2;

      if (is_ok(m)) {
         ok = m;
      } else {
         ng = m;
      }
   }
   // 変数okが条件を満たす最大/最小になっている

   // [End] Binary Search(Double)
}

void Shakutori() {
   int N = 0;
   bool __DUMMY__ = true;

   // [Start] Shakutori method
   // [Prefix] shakutori-inline
   // しゃくとり法
   // 区間は半開区間[l, r)で考える(0 <= l < r <= N)
   int r = 0;

   for (int l = 0; l < N; l++) {
      while (r < N && __DUMMY__ /* 開区間をr+1まで延ばせるか、つまりrが条件を満たすかチェック */) {
         // rを進める
         r++;
      }

      // rはlを固定した時に条件を満たす最大値になっているので操作を行う
      // ex. cnt += (r - l);

      // lをインクリメントする準備
      if (l == r) {
         // l, rが重なる場合はrも動かす
         r++;
      } else {
         // lのみが更新される
      }
   }
   // [End] Shakutori method
}

// [Start] Max v (v <= Val) in vector
// [Prefix] find-max-leq-vec-func
// [Verified] N, Q<= 5*10^5: Static Range Frequency(https://judge.yosupo.jp/problem/static_range_frequency)
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
// [End] Max v (v <= Val) in vector

// [Start] Max v (v <= Val) in set
// [Prefix] find-max-leq-set-func
// val以下で最大の要素を返す
// @note 条件を満たす要素がない場合はendイテレータを返す
template <class T>
typename set<T>::const_iterator find_max_less_eq(const set<T>& val_set, const T& val) {
   // valより大きい要素のひとつ前の要素 = val以下で最大の要素
   auto it = val_set.upper_bound(val);

   if (it == val_set.begin()) {
      return val_set.end();
   }

   --it;

   return it;
}
// [End] Max v (v <= Val) in set

// [Start] Max v (v < Val) in vector
// [Prefix] find-max-less-vec-func
// [Verified] N, Q<= 5*10^5: Static Range Frequency(https://judge.yosupo.jp/problem/static_range_frequency)
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
// [End] Max v (v < Val) in vector

// [Start] Max v (v < Val) in set
// [Prefix] find-max-less-set-func
// valより小さい最大の要素を返す
// @note 条件を満たす要素がない場合はendイテレータを返す
template <class T>
typename set<T>::const_iterator find_max_less(const set<T>& val_set, const T& val) {
   // val以上の要素のひとつ前の要素 = valより小さい最大の要素
   auto it = val_set.lower_bound(val);

   if (it == val_set.begin()) {
      return val_set.end();
   }

   --it;

   return it;
}
// [End] Max v (v < Val) in set

// [Start] Min v (v >= Val) in vector
// [Prefix] find-min-geq-vec-func
// [Verified] N, Q<= 5*10^5: Static Range Frequency(https://judge.yosupo.jp/problem/static_range_frequency)
// val以上で最小の要素を返す
// @note 条件を満たす要素がない場合はendイテレータを返す
// @note val_listはソート済であること
template <class T>
typename vector<T>::const_iterator
find_min_greater_eq(const vector<T>& val_list, const T& val) {
   auto it = lower_bound(val_list.begin(), val_list.end(), val);
   return it;
}
// [End] Min v (v >= Val) in vector

// [Start] Min v (v >= Val) in set
// [Prefix] find-min-geq-set-func
// val以上で最小の要素を返す
// @note 条件を満たす要素がない場合はendイテレータを返す
template <class T>
typename set<T>::const_iterator find_min_greater_eq(const set<T>& val_set, const T& val) {
   auto it = val_set.lower_bound(val);
   return it;
}
// [End] Min v (v >= Val) in set

// [Start] Min v (v > Val) in vector
// [Prefix] find-min-greater-vec-func
// valより大きい最小の要素を返す
// @note 条件を満たす要素がない場合はendイテレータを返す
// @note val_listはソート済であること
template <class T>
typename vector<T>::const_iterator find_min_greater(const vector<T>& val_list, const T& val) {
   auto it = upper_bound(val_list.begin(), val_list.end(), val);
   return it;
}
// [End] Min v (v > Val) in vector

// [Start] Min v (v > Val) in set
// [Prefix] find-min-greater-set-func
// valより大きい最小の要素を返す
// @note 条件を満たす要素がない場合はendイテレータを返す
template <class T>
typename set<T>::const_iterator find_min_greater(const set<T>& val_set, const T& val) {
   auto it = val_set.upper_bound(val);
   return it;
}
// [End] Min v (v > Val) in set
